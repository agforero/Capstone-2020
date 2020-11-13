//Holding JSON data

let MockFrame = {
	"nodes": {
		"0": {
			"id": 0,
			"name": "A",
			"position": { "x": 50, "y": 50 },
			"shape": "circle",
			"greyedOut": false,
			"color": "#fff",
			"texture": "normal",
			"annotation": ""
		},
		"1": {
			"id": 1,
			"name": "B",
			"position": { "x": 400, "y": 50 },
			"shape": "circle",
			"greyedOut": false,
			"color": "#fff",
			"texture": "normal",
			"annotation": ""
		},
		"2": {
			"id": 2,
			"name": "C",
			"position": { "x": 200, "y": 200 },
			"shape": "circle",
			"greyedOut": false,
			"color": "#fff",
			"texture": "normal",
			"annotation": ""
		}
	},
	"edges": {
		"0-1": {
			"start": 0,
			"end": 1,
			"weight": 0,
			"reversed": false,
			"color": "#000",
			"thick": false,
			"greyedOut": false
		},
		"0-2": {
			"start": 0,
			"end": 2,
			"weight": 0,
			"reversed": false,
			"color": "#000",
			"thick": false,
			"greyedOut": false
		},
		"1-2": {
			"start": 1,
			"end": 2,
			"weight": 0,
			"reversed": false,
			"color": "#000",
			"thick": false,
			"greyedOut": false
		},
		"1-0": {
			"start": 1,
			"end": 0,
			"weight": 0,
			"reversed": false,
			"color": "#000",
			"thick": false,
			"greyedOut": false
		},
		"2-0": {
			"start": 2,
			"end": 0,
			"weight": 0,
			"reversed": false,
			"color": "#000",
			"thick": false,
			"greyedOut": false
		},
		"2-1": {
			"start": 2,
			"end": 1,
			"weight": 0,
			"reversed": false,
			"color": "#000",
			"thick": false,
			"greyedOut": false
		}
	},
	"groups": [
		{
			"id": 0,
			"members": [
				0,
				1,
				2
			],
			"color": "#00a600",
			"dashBorder": false
		}
	]
}

export default MockFrame;