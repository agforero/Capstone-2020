const Textures = Object.freeze({
	"normal": 1,
	"filled": 2,
	"squares": 3,
	"dot": 4,
	"cross": 5,
});
const Shapes = Object.freeze({
	"circle": 1,
	"square": 2,
	"triangle": 3,
	"star": 4,
});

class Node {
	constructor(id, name, x = 0, y = 0) {
		this.id = id;
		this.name = name;
		this.position = { x: x, y: y };
		this.texture = Textures.normal;
		this.color = "#fff";
		this.shape = Shapes.circle;
		this.annotation = "";
		this.greyedOut = false;
	}
}

class Edges {
	constructor(start, end) {
		this.start = start;
		this.end = end;
		this.weight = 0;
		this.reversed = false;
		this.greyedOut = false;
		this.color = "#000";
		this.thick = false;
	}
}

class Group {
	constructor(nodes = [], color = "#00a600") {
		this.members = nodes;
		this.color = color;
		this.dashBorder = false;
	}
}