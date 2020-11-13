//Holding JSON data

let Frames = [
	{
		"nodes": {
			"0": {
				"id": 0, // are the id's really needed? when would you have access to this without knowing the key used to access it in the object?
				"name": "A",
				"position": { "x": 50, "y": 50 },
				"color": "#00a600",
				"annotation":"0",
			},
			"1": {
				"id": 1,
				"name": "B",
				"position": { "x": 150, "y": 50 },
				"color": "#f00",
				"annotation":"1?",
			},
			"2": {
				"id": 2,
				"name": "C",
				"position": { "x": 250, "y": 50 },
				"color": "#f00",
				"annotation":"inf?",
			},
			"3":{
				"id":3,
				"name":"D",
				"position":{"x":50,"y":100},
				"color": "#f00",
				"annotation":"2?",
			},
			"4":{
				"id":4,
				"name":"E",
				"position":{"x":150,"y":100},
				"color": "#f00",
				"annotation":"3?",
			}
		},
		"edges": {
			"0-1": { //keys are where they're from-to because then we can be sure that they're referencing the same one between frames (right?)
				"start": 0,
				"end": 1,
				"weight": 1,
			},
			"0-3": {
				"start": 0,
				"end": 3,
				"weight": 2,
			},
			"0-4":{
				"start":0,
				"end":4,
				"weight":3,
			},
			"1-2": {
				"start": 1,
				"end": 2,
				"weight": 2,
			},
			"1-4":{
				"start":1,
				"end":4,
				"weight":1,
			},
		},
	},

	{
		"nodes": {
			"0": {
				"id": 0, // are the id's really needed? when would you have access to this without knowing the key used to access it in the object?
				"name": "A",
				"position": { "x": 50, "y": 50 },
				"color": "#00a600",
				"annotation":"0",
			},
			"1": {
				"id": 1,
				"name": "B",
				"position": { "x": 150, "y": 50 },
				"color": "#00a600",
				"annotation":"1",
			},
			"2": {
				"id": 2,
				"name": "C",
				"position": { "x": 250, "y": 50 },
				"color": "#f00",
				"annotation":"3?",
			},
			"3":{
				"id":3,
				"name":"D",
				"position":{"x":50,"y":100},
				"color": "#f00",
				"annotation":"2?",
			},
			"4":{
				"id":4,
				"name":"E",
				"position":{"x":150,"y":100},
				"color": "#f00",
				"annotation":"2?",
			}
		},
		"edges": {
			"0-1": { //keys are where they're from-to because then we can be sure that they're referencing the same one between frames (right?)
				"start": 0,
				"end": 1,
				"weight": 1,
			},
			"0-3": {
				"start": 0,
				"end": 3,
				"weight": 2,
			},
			"0-4":{
				"start":0,
				"end":4,
				"weight":3,
			},
			"1-2": {
				"start": 1,
				"end": 2,
				"weight": 2,
			},
			"1-4":{
				"start":1,
				"end":4,
				"weight":1,
			},
		},
	},
]

export default Frames;