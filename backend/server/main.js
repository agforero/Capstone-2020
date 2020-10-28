/*
Code for the server
For now, we'll only worry about having one session open.
Future options - either:
  - Try to get the code to work for multiple sessions
  - Containerize this and have Kubernetes spin up another for each session
*/

'use strict';

const express = require('express');
const app = express();
var http = require('http').createServer(app);
var io = require('socket.io')(http);
const { deepEqual } = require('assert');
const path = require('path');
const fs = require('fs');
const { exit } = require('process');
const { json } = require('express');
const util = require('util');

// ---------- Consts ----------
const EXPECTED_USERS = 2; // Sent to this page from another, this is how many were in the graph creation page

// ---------- Vars ----------
var frames; // The generated frames (and pop quizzes, etc)
var frameCount = 0; // Current frame
var users = []; // List of users (JS objects, NOTE: there are a lot of undefined things in the list as users disconnect so it's easier to keep track of the Clicker)
var clickerIndex = null; // Index in users of the Clicker
var expectedAnswer = null; // Will be filled with an object that represents the correct answer to pop quizzes

// ---------- Setup Code ----------
// TODO: call C++ things to generate it
const dataFile = 'data/dijkstra_complete.json';

// ---------- Start Server ----------
// Add ability to serve the "build" directory
// Symlinked from the React build (made with `yarn build` command)
app.use(express.static(path.join(__dirname, 'build')));

// Serve index.html from there
app.get('/', (req, res) => {
	res.sendFile(path.join(__dirname, 'build', 'index.html'));
});

// ---------- Socket.IO Code ----------
io.on('connection', (socket) => {
	// ---------- Setup for new user connected ----------
	users.push({
		id: socket.id, // For now it's just the socket's unique ID, nothing to do with the index in the users array
		//socket: socket,
	});
	socket.index = users.length - 1; // Keep track of which user this is

	socket.emit('assign_id', users.length - 1);
	console.log("user " + socket.index + " connected");

	// ---------- Socket Functions ----------
	socket.on('disconnect', () => {
		let id = socket.index;
		console.log("user " + id + " disconnected");
		let index = null;
		for (let i = 0; i < users.length; i++) {
			if (users[i] == null) {
				continue;
			}
			if (users[i].id === id) {
				// Make that index null, but don't delete the null val from array for ease of keeping track of who the Clicker is for now
				delete users[i];
				break;
			}
		}
	});

	socket.on('request_animation_start', () => {
		if (users.length >= EXPECTED_USERS) {
			selectClicker();
			startAnimation();
		}
	});

	// User requested next frame
	socket.on('request_next_frame', () => {
		if (socket.index != clickerIndex) {
			// Not the clicker, ignore the spam
			return;
		}

		// At last frame, so just keep sending that when they ask for it
		if (frameCount >= frames.length) {
			io.emit('send_next_frame', frames[frames.length - 1]);
			return;
		}
		let nextItem = frames[frameCount++];
		switch (nextItem.type) {
			case "animation":
			case "end":
				// Send it to frontend as frame
				io.emit('send_next_frame', nextItem);
				break;
			case "popQuiz_Question":
				console.log("ERROR: THIS SHOULDN'T COME UP THIS WAY, WE SHOULD CHECK FOR IT IMMEDIATELY AFTER RECEIVING AN ANSWER FROM THE USER");
				break;
			case "popQuiz_Answer":
				expectedAnswer = nextItem.answer;
				// Ask question
				io.emit('pop_quiz_question', frames[frameCount++]);
				break;
			default:
				console.log("ERROR: UNKNOWN TYPE: " + nextItem.type);
		}
	});

	socket.on('add_doodle', (doodle) => {
		// Forward to all other users
		socket.broadcast.emit('forward_doodle', doodle);
	});

	socket.on('make_change', (change) => {
		if (socket.index != clickerIndex) {
			// Not the clicker, ignore the spam
			return;
		}
		socket.broadcast.emit('made_change', change);
	});

	// User submitted an answer, grade it
	socket.on('pop_quiz_answer', (changes) => {
		if (socket.index != clickerIndex) {
			// Not the clicker, ignore the spam
			return;
		}

		pruneEmpty(changes);
		//let correctAnswer = frames[frameCount++].answer; // Answers should always follow the questions in the frame JSON list
		if (gradeAnswer(changes, expectedAnswer)) {
			console.log("correct!")
			io.emit('pop_quiz_grade_correct');
		} else {
			io.emit('pop_quiz_grade_incorrect');
			//console.log("incorrect!");
			//console.log("expected:");
			//console.log(util.inspect(expectedAnswer, { depth: null }));
			//console.log("but got:");
			//console.log(util.inspect(changes, { depth: null }));
			selectClicker();
		}
	});

});

// ---------- Open Server ----------
http.listen(8888, () => {
	console.log('listening on *:8888');
});

// ---------- Helper Funcs ----------

function startAnimation() {
	io.emit('ready_to_start');
	fs.readFile(dataFile, 'utf8', (err, data) => {
		if (err) {
			console.log("ERROR: can't read JSON file: " + dataFile);
			exit(1);
		}
		frames = JSON.parse(data);
		frameCount = 0;
		io.emit('send_next_frame', frames[frameCount++]);
	});
}

function selectClicker() {
	// Note: this randomly selects anyone as long as they're not the only one. Could hypothetically just cycle between two users and some may never be the clicker

	// If there's only one user, don't bother picking a new one
	if (EXPECTED_USERS == 1) {
		return;
	}
	let oldClickerIndex = clickerIndex;
	let newClickerIndex = null;
	do {
		newClickerIndex = Math.floor(Math.random() * users.length);
	} while (newClickerIndex == null || users[newClickerIndex] == null || (newClickerIndex === oldClickerIndex));
	clickerIndex = newClickerIndex;
	io.emit('new_clicker', newClickerIndex);
}

// Delete all {} from object
function pruneEmpty(obj) {
	for (const key in obj) {
		let val = obj[key];
		if (typeof val === "object" && val != null) {
			if (Object.keys(val).length === 0) {
				delete obj[key];
			} else {
				pruneEmpty(obj[key]);
			}
		}
	}
}


// {} and null should count as empty, and so should things like a:{b:{c:{}}}
function isEmpty(obj) {
	if (obj == null) {
		return true;
	} else if (typeof obj !== "object") {
		return false;
	}

	for (const key in obj) {
		if (typeof obj[key] !== "object") {
			return false;
		} else {
			if (!isEmpty(obj[key])) {
				return false;
			}
		}
	}
	return true;
}

// Recursively compare objects to test if equal, used to compare hardcoded answer with user answers
// Might be a good idea to return a count of differences along with a count of total size
function gradeAnswer(object1, object2) {
	if (object1 === null || isEmpty(object1)) {
		if (object2 === null || isEmpty(object2)) {
			// both empty
			return true;
		} else {
			// first empty, second not
			return false;
		}
	} else if (object2 === null || isEmpty(object2)) {
		// second empty, first not
		return false;
	}
	const keys1 = Object.keys(object1);
	const keys2 = Object.keys(object2);

	if (keys1.length !== keys2.length) {
		// unequal keys length
		return false;
	}

	for (const key of keys1) {
		const val1 = object1[key];
		const val2 = object2[key];
		const areObjects = (typeof val1 === 'object') && (typeof val2 === 'object');
		if (
			(areObjects && !gradeAnswer(val1, val2)) ||
			(!areObjects && val1 !== val2)
		) {
			// values didn't match (or recursively failed)
			return false;
		}
	}

	return true;
}