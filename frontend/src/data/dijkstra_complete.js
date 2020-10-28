//Holding JSON data

let Frames = [
	{
		"type": "animation",
		"nodes": {
			"0": {
				"id": 0,
				"name": "A",
				"position": { "x": 50, "y": 50 },
				"color": "#0f0",
				"annotation": "0",
			},
			"1": {
				"id": 1,
				"name": "B",
				"position": { "x": 150, "y": 50 },
				"color": "#f00",
				"annotation": "1?",
			},
			"2": {
				"id": 2,
				"name": "C",
				"position": { "x": 250, "y": 50 },
				"color": "#f00",
				"annotation": "inf?",
			},
			"3": {
				"id": 3,
				"name": "D",
				"position": { "x": 50, "y": 100 },
				"color": "#f00",
				"annotation": "2?",
			},
			"4": {
				"id": 4,
				"name": "E",
				"position": { "x": 150, "y": 100 },
				"color": "#f00",
				"annotation": "3?",
			},
			"5": {
				"id": 5,
				"name": "F",
				"position": { "x": 250, "y": 100 },
				"color": "#f00",
				"annotation": "inf?"
			},
		},
		"edges": {
			"0-1": {
				"id": "0-1",
				"start": 0,
				"end": 1,
				"weight": 1,
			},
			"0-3": {
				"id": "0-3",
				"start": 0,
				"end": 3,
				"weight": 2,
			},
			"0-4": {
				"id": "0-4",
				"start": 0,
				"end": 4,
				"weight": 3,
			},
			"1-2": {
				"id": "1-2",
				"start": 1,
				"end": 2,
				"weight": 2,
			},
			"1-4": {
				"id": "1-4",
				"start": 1,
				"end": 4,
				"weight": 1,
			},
			"1-5": {
				"id": "1-5",
				"start": 1,
				"end": 5,
				"weight": 8,
			},
			"4-5": {
				"id": "4-5",
				"start": 4,
				"end": 5,
				"weight": 2,
			},
		},
	},

	{
		"type": "animation",
		"nodes": {
			"0": {
				"id": 0,
				"name": "A",
				"position": { "x": 50, "y": 50 },
				"color": "#0f0",
				"annotation": "0",
			},
			"1": {
				"id": 1,
				"name": "B",
				"position": { "x": 150, "y": 50 },
				"color": "#0f0",
				"annotation": "1",
			},
			"2": {
				"id": 2,
				"name": "C",
				"position": { "x": 250, "y": 50 },
				"color": "#f00",
				"annotation": "3?",
			},
			"3": {
				"id": 3,
				"name": "D",
				"position": { "x": 50, "y": 100 },
				"color": "#f00",
				"annotation": "2?",
			},
			"4": {
				"id": 4,
				"name": "E",
				"position": { "x": 150, "y": 100 },
				"color": "#f00",
				"annotation": "2?",
			},
			"5": {
				"id": 5,
				"name": "F",
				"position": { "x": 250, "y": 100 },
				"color": "#f00",
				"annotation": "9?"
			},
		},
		"edges": {
			"0-1": {
				"id": "0-1",
				"start": 0,
				"end": 1,
				"weight": 1,
			},
			"0-3": {
				"id": "0-3",
				"start": 0,
				"end": 3,
				"weight": 2,
			},
			"0-4": {
				"id": "0-4",
				"start": 0,
				"end": 4,
				"weight": 3,
			},
			"1-2": {
				"id": "1-2",
				"start": 1,
				"end": 2,
				"weight": 2,
			},
			"1-4": {
				"id": "1-4",
				"start": 1,
				"end": 4,
				"weight": 1,
			},
			"1-5": {
				"id": "1-5",
				"start": 1,
				"end": 5,
				"weight": 8,
			},
			"4-5": {
				"id": "4-5",
				"start": 4,
				"end": 5,
				"weight": 2,
			},
		},
	},

	{
		"type": "popQuiz_Answer",
		"answer": {
			"edges": {
				"0-1": {
					"color": "blue",
				},
				"1-4": {
					"color": "blue",
				},
				"4-5": {
					"color": "blue",
				},
			}
		}
	},
	{
		"type": "popQuiz_Question",
		"text": "We've just discovered a 9-long path to node F. Highlight a fastest path to F that will be discovered later. (Note: for now, just click on all relevant edges) (Note: if there are multiple optimal paths, for now you have to select the one the algorithm will wind up selecting)",
		// Note: might be a better idea to evaluate the path length on the fly to see if they found an optimal-length path
		"question": "dijkstra_highlightPath",
	},

	{
		"type": "animation",
		"nodes": {
			"0": {
				"id": 0,
				"name": "A",
				"position": { "x": 50, "y": 50 },
				"color": "#0f0",
				"annotation": "0",
			},
			"1": {
				"id": 1,
				"name": "B",
				"position": { "x": 150, "y": 50 },
				"color": "#0f0",
				"annotation": "1",
			},
			"2": {
				"id": 2,
				"name": "C",
				"position": { "x": 250, "y": 50 },
				"color": "#f00",
				"annotation": "3?",
			},
			"3": {
				"id": 3,
				"name": "D",
				"position": { "x": 50, "y": 100 },
				"color": "#0f0",
				"annotation": "2",
			},
			"4": {
				"id": 4,
				"name": "E",
				"position": { "x": 150, "y": 100 },
				"color": "#f00",
				"annotation": "2?",
			},
			"5": {
				"id": 5,
				"name": "F",
				"position": { "x": 250, "y": 100 },
				"color": "#f00",
				"annotation": "9?"
			},
		},
		"edges": {
			"0-1": {
				"id": "0-1",
				"start": 0,
				"end": 1,
				"weight": 1,
			},
			"0-3": {
				"id": "0-3",
				"start": 0,
				"end": 3,
				"weight": 2,
			},
			"0-4": {
				"id": "0-4",
				"start": 0,
				"end": 4,
				"weight": 3,
			},
			"1-2": {
				"id": "1-2",
				"start": 1,
				"end": 2,
				"weight": 2,
			},
			"1-4": {
				"id": "1-4",
				"start": 1,
				"end": 4,
				"weight": 1,
			},
			"1-5": {
				"id": "1-5",
				"start": 1,
				"end": 5,
				"weight": 8,
			},
			"4-5": {
				"id": "4-5",
				"start": 4,
				"end": 5,
				"weight": 2,
			},
		},
	},

	// Have answers before questions for now to save frontend from being too messy (temporary solution until backend is made)
	{
		"type": "popQuiz_Answer",
		"answer": {
			"nodes": {
				"4": {
					"annotation": "2",
					"color": "blue",
				},
				"5": {
					"annotation": "4?",
				}
			}
		}
	},
	{
		"type": "popQuiz_Question",
		"text": "Circle the node that will be explored next, and update all relevant distances (Note: node distances aren't set in stone until the node is explored...).",
		"question": "dijkstra_updateNeighbors",
	},

	{
		"type": "animation",
		"nodes": {
			"0": {
				"id": 0,
				"name": "A",
				"position": { "x": 50, "y": 50 },
				"color": "#0f0",
				"annotation": "0",
			},
			"1": {
				"id": 1,
				"name": "B",
				"position": { "x": 150, "y": 50 },
				"color": "#0f0",
				"annotation": "1",
			},
			"2": {
				"id": 2,
				"name": "C",
				"position": { "x": 250, "y": 50 },
				"color": "#f00",
				"annotation": "3?",
			},
			"3": {
				"id": 3,
				"name": "D",
				"position": { "x": 50, "y": 100 },
				"color": "#0f0",
				"annotation": "2",
			},
			"4": {
				"id": 4,
				"name": "E",
				"position": { "x": 150, "y": 100 },
				"color": "#0f0",
				"annotation": "2",
			},
			"5": {
				"id": 5,
				"name": "F",
				"position": { "x": 250, "y": 100 },
				"color": "#f00",
				"annotation": "4?"
			},
		},
		"edges": {
			"0-1": {
				"id": "0-1",
				"start": 0,
				"end": 1,
				"weight": 1,
			},
			"0-3": {
				"id": "0-3",
				"start": 0,
				"end": 3,
				"weight": 2,
			},
			"0-4": {
				"id": "0-4",
				"start": 0,
				"end": 4,
				"weight": 3,
			},
			"1-2": {
				"id": "1-2",
				"start": 1,
				"end": 2,
				"weight": 2,
			},
			"1-4": {
				"id": "1-4",
				"start": 1,
				"end": 4,
				"weight": 1,
			},
			"1-5": {
				"id": "1-5",
				"start": 1,
				"end": 5,
				"weight": 8,
			},
			"4-5": {
				"id": "4-5",
				"start": 4,
				"end": 5,
				"weight": 2,
			},
		},
	},

	{
		"type": "animation",
		"nodes": {
			"0": {
				"id": 0,
				"name": "A",
				"position": { "x": 50, "y": 50 },
				"color": "#0f0",
				"annotation": "0",
			},
			"1": {
				"id": 1,
				"name": "B",
				"position": { "x": 150, "y": 50 },
				"color": "#0f0",
				"annotation": "1",
			},
			"2": {
				"id": 2,
				"name": "C",
				"position": { "x": 250, "y": 50 },
				"color": "#0f0",
				"annotation": "3",
			},
			"3": {
				"id": 3,
				"name": "D",
				"position": { "x": 50, "y": 100 },
				"color": "#0f0",
				"annotation": "2",
			},
			"4": {
				"id": 4,
				"name": "E",
				"position": { "x": 150, "y": 100 },
				"color": "#0f0",
				"annotation": "2",
			},
			"5": {
				"id": 5,
				"name": "F",
				"position": { "x": 250, "y": 100 },
				"color": "#f00",
				"annotation": "4?"
			},
		},
		"edges": {
			"0-1": {
				"id": "0-1",
				"start": 0,
				"end": 1,
				"weight": 1,
			},
			"0-3": {
				"id": "0-3",
				"start": 0,
				"end": 3,
				"weight": 2,
			},
			"0-4": {
				"id": "0-4",
				"start": 0,
				"end": 4,
				"weight": 3,
			},
			"1-2": {
				"id": "1-2",
				"start": 1,
				"end": 2,
				"weight": 2,
			},
			"1-4": {
				"id": "1-4",
				"start": 1,
				"end": 4,
				"weight": 1,
			},
			"1-5": {
				"id": "1-5",
				"start": 1,
				"end": 5,
				"weight": 8,
			},
			"4-5": {
				"id": "4-5",
				"start": 4,
				"end": 5,
				"weight": 2,
			},
		},
	},

	{
		"type": "animation",
		"nodes": {
			"0": {
				"id": 0,
				"name": "A",
				"position": { "x": 50, "y": 50 },
				"color": "#0f0",
				"annotation": "0",
			},
			"1": {
				"id": 1,
				"name": "B",
				"position": { "x": 150, "y": 50 },
				"color": "#0f0",
				"annotation": "1",
			},
			"2": {
				"id": 2,
				"name": "C",
				"position": { "x": 250, "y": 50 },
				"color": "#0f0",
				"annotation": "3",
			},
			"3": {
				"id": 3,
				"name": "D",
				"position": { "x": 50, "y": 100 },
				"color": "#0f0",
				"annotation": "2",
			},
			"4": {
				"id": 4,
				"name": "E",
				"position": { "x": 150, "y": 100 },
				"color": "#0f0",
				"annotation": "2",
			},
			"5": {
				"id": 5,
				"name": "F",
				"position": { "x": 250, "y": 100 },
				"color": "#0f0",
				"annotation": "4"
			},
		},
		"edges": {
			"0-1": {
				"id": "0-1",
				"start": 0,
				"end": 1,
				"weight": 1,
			},
			"0-3": {
				"id": "0-3",
				"start": 0,
				"end": 3,
				"weight": 2,
			},
			"0-4": {
				"id": "0-4",
				"start": 0,
				"end": 4,
				"weight": 3,
			},
			"1-2": {
				"id": "1-2",
				"start": 1,
				"end": 2,
				"weight": 2,
			},
			"1-4": {
				"id": "1-4",
				"start": 1,
				"end": 4,
				"weight": 1,
			},
			"1-5": {
				"id": "1-5",
				"start": 1,
				"end": 5,
				"weight": 8,
			},
			"4-5": {
				"id": "4-5",
				"start": 4,
				"end": 5,
				"weight": 2,
			},
		},
	},

	{
		"type": "end",
	}
]

export default Frames;