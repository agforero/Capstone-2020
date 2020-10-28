import React from 'react';
import Graph from './components/Graph';
import socketIOClient from "socket.io-client";

// Just for testing graph generation
import Frames from './data/agustin.json';

//const SERVER_IP = 'http://localhost:8888';
const SERVER_IP = 'http://rns203-4.cs.stolaf.edu:8888';

const TESTING_GRAPH_GENERATION = false;

// TODO: attach doodles to frame so they're still there later??

class App extends React.Component {
	// Handles networking and frames
	constructor(props) {
		super(props);

		// Set vars
		this.state = {
			frame: 0, // Current frame
			changeSet: {}, // Object to keep track of what changed. For example, if the object was {nodes:{1:{annotation:"asdf"}}} then the annotation for nodes[1] should be crossed out, and a new one saying "asdf" should be placed on top of it. Should be just like the frame object but with things changed by the user (for now at least)
			popQuiz: null, // Type of pop quiz we're currently looking at (ex: "dijkstra_highlightPath")
			question: "", // Pop quiz text
			resetDoodles: false, // Boolean that switches back and forth whenever the child doodles should be deleted
			readyToStart: false, // Everyone's in, start up the animation
			id: null, // User id in server (index in users array)
			clickerID: null, // ID of the clicker
		}

		this.frames = [{}]; // Frames of the animation, starts with empty one
		if (TESTING_GRAPH_GENERATION) {
			this.frames = Frames; // Generating test
		}

		this.otherDoodles = []; // Doodles from other users

		// So that functions to know what "this" means
		this.requestFrame = this.requestFrame.bind(this);
		this.makeChange = this.makeChange.bind(this);
		this.submitToGrade = this.submitToGrade.bind(this);
		this.isClicker = this.isClicker.bind(this);
		this.resetDoodles = this.resetDoodles.bind(this);

		// Socket setup
		this.socket = socketIOClient(SERVER_IP);

		// Enough people have joined and the animation is ready to start
		this.socket.on('ready_to_start', () => {
			this.setState({ readyToStart: true });
		});

		// Server sent over a frame
		this.socket.on('send_next_frame', (data) => {
			//console.log(data);
			switch (data.type) {
				case "animation":
					this.frames.push(data);
					this.setState({
						frame: this.state.frame + 1,
						popQuiz: null,
					});
					// Don't care about old doodles anymore
					this.resetDoodles();
					break;
				case "end":
					alert("Finished!");
					break;
				default:
					console.log("UNKNOWN FRAME TYPE: " + data.type);
			}
		});

		// Server sent a pop quiz question
		this.socket.on('pop_quiz_question', (data) => {
			this.setState({
				popQuiz: data.question,
				question: data.text,
			});
			this.resetDoodles();
		});

		this.socket.on('forward_doodle', (doodle) => {
			// TODO: set state somehow so it actually updates
			this.otherDoodles.push(doodle);
		});

		this.socket.on('made_change', (change) => {
			this.makeChange(change);
		});

		// Server said it's correct
		this.socket.on('pop_quiz_grade_correct', () => {
			this.setState({
				changeSet: {},
				popQuiz: null,
			});
			//this.requestFrame(); // Request next frame immediately (commented out for now)
		});

		// Server said it's incorrect
		this.socket.on('pop_quiz_grade_incorrect', () => {
			this.setState({
				changeSet: {},
			});
		});

		this.socket.on('assign_id', (id) => {
			this.setState({ id: id });
		});

		this.socket.on('new_clicker', (id) => {
			this.setState({ clickerID: id });
		});
	}

	requestFrame() {
		console.log("requesting frame");
		this.socket.emit('request_next_frame');
	}

	makeChange(change) {
		if (!this.isClicker() || this.popQuiz != null) {
			// Don't allow changes to go through
			return;
		}
		let tempChanges = this.state.changeSet;
		mergeObjects(tempChanges, change);
		this.setState({ changeSet: tempChanges });
	}

	// Temporary function before we have grading happen on the backend
	submitToGrade(changes) {
		this.socket.emit('pop_quiz_answer', changes);
	}

	isClicker() {
		return this.state.id === this.state.clickerID;
	}

	resetDoodles() {
		// Reset doodles from other users
		this.otherDoodles = [];
		// Reset child doodles from user
		this.setState({ resetDoodles: !this.state.resetDoodles });
	}

	render() {
		if (!this.state.readyToStart && !TESTING_GRAPH_GENERATION) {
			return (<div>
				<p>User ID: {this.state.id}</p>
				<p>Click "start" when all users are in.</p>
				<button
					onClick={() => {
						this.socket.emit('request_animation_start');
					}}>Start</button>
			</div>)
		}

		return (<div
			margin="0px"
			padding="0px"
		>
			<Graph
				frame={this.frames[this.state.frame]}
				addChange={(change) => {
					if (!this.isClicker()) {
						return;
					}
					this.makeChange(change);
					this.socket.emit('make_change', change);
				}}
				addDoodle={(doodle) => {
					this.socket.emit('add_doodle', doodle);
				}}
				otherDoodles={this.otherDoodles}
				resetDoodles={this.state.resetDoodles}
				question={this.state.popQuiz}
				changes={this.state.changeSet}
			/>
			<button
				onClick={() => {
					this.setState({ frame: Math.max(0, this.state.frame - 1) })
				}}
				disabled={this.state.popQuiz != null /*|| !this.isClicker()*/}
			>
				Previous
			</button>
			<button
				onClick={() => {
					// If we're at the last frame we have loaded in, request another
					if (this.state.frame === this.frames.length - 1) {
						this.requestFrame();
					} else {
						this.setState({ frame: this.state.frame + 1 });
					}
				}}
				disabled={this.state.popQuiz != null /*|| !this.isClicker()*/}
			>
				Next
			</button>
			<br /><br />
			<button
				onClick={() => {
					//console.log("SENDING CHANGESET TO BACKEND:")
					//console.log(this.state.changeSet);
					this.submitToGrade(this.state.changeSet);
				}}
				disabled={this.state.popQuiz == null || !this.isClicker()}
			>
				Submit
			</button>
			<p>ID: {this.state.id}</p>
			{
				this.isClicker() ?
					<p><b>You're the Clicker!</b></p>
					:
					<p>Clicker id: {this.state.clickerID}</p>
			}
			{
				this.state.popQuiz != null ?
					<p>Goal: {this.state.question}</p>
					:
					null
			}
		</div >);
	}
}

// Recursively merge objects, such as changesets
function mergeObjects(a, b) {
	if (typeof a === "undefined") {
		a = {};
	}
	if (typeof a !== "object" || typeof b !== "object") {
		console.log(a);
		console.log(b);
		throw new Error("ERROR: non-object handed to mergeObjects");
	}
	for (const att in b) {
		if (typeof b[att] === "object") {
			if (a[att] === null) {
				a[att] = {};
			}
			a[att] = mergeObjects(a[att], b[att]);
		} else {
			if (a[att] === b[att]) { // Undo changes
				delete a[att];
				console.log("removed " + att + " " + b[att] + " from object");
			} else {
				a[att] = b[att];
				console.log("added " + att + " " + b[att] + " to object");
			}
		}
	}
	return a;
}

export default App;