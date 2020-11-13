import React from 'react';

const dijkstra = [
	"function Dijkstra(Graph, source):", // 0
	"⠀⠀for each vertex v in Graph:", // 1
	"⠀⠀⠀⠀dist[v] := infinity", // 2
	"⠀⠀⠀⠀previous[v] := undefined", // 3
	"⠀⠀dist[source] := 0", // 4
	"⠀⠀Q := the set of all nodes in Graph", // 5
	"⠀⠀while Q is not empty:", // 6
	"⠀⠀⠀⠀u := node in Q with min dist[]", // 7
	"⠀⠀⠀⠀remove u from Q", // 8
	"⠀⠀⠀⠀for each neighbor v of u:", // 9
	"⠀⠀⠀⠀⠀⠀alt := dist[u] + dist_between(u,v)", // 10
	"⠀⠀⠀⠀⠀⠀if alt < dist[v]", // 11
	"⠀⠀⠀⠀⠀⠀⠀⠀dist[v] := alt", // 12
	"⠀⠀⠀⠀⠀⠀⠀⠀previous[v] := u", // 13
	"⠀⠀return previous[]" // 14
];

class Pseudocode extends React.Component {
	constructor(props) {
		super(props);

		// TODO: add others
		this.pseudocode = null;
		switch (this.props.algo) {
			case 'dijkstra':
				this.pseudocode = dijkstra;
				break;
			default:
				this.pseudocode = [];
		}

		// TODO: pass frame number too, so it knows when to increase the count and when it's just rerendering because of other user input
		this.state = {
			code: this.initList(this.pseudocode),
		};

		this.highestFrame = -1 * Infinity;

		this.initList = this.initList.bind(this);
		//this.recursiveListRender = this.recursiveListRender.bind(this);
	}

	initList(code) {
		let ret = [];
		for (let i = 0; i < code.length; i++) {
			ret.push({
				count: 0,
				line: code[i],
			});
		}
		return ret;
	}

	dynamicWhitespace(n) {
		// returns 1 whitespace if n is singledigit;
		// 0 whitespaces if n is double digit.
		return (n < 10) ? " " : ""
	}

	componentDidUpdate() {
		if (this.props.hide || this.props.currentLine == null) {
			return null;
		}
		if (this.props.frameNumber > this.highestFrame) {
			//console.log("update highestFrame");
			this.highestFrame = this.props.frameNumber;
			let temp = this.state.code;
			temp[this.props.currentLine].count++;
			this.setState({ code: temp });
		}
	}

	render() {
		if (this.props.hide) {
			return null;
		}

		let displayCode = [];
		for (let i = 0; i < this.state.code.length; i++) {
			displayCode.push(<p style={{
				margin: 0,
				padding: 3,
				width: 494,
				whiteSpace: "normal",
				fontFamily: "Courier New",
				fontSize: 16,
				backgroundColor: this.props.currentLine === i ? "lightgrey" : "white"
			}}>
				{(this.state.code[i].count === 0 ? "⠀" : this.state.code[i].count) + this.dynamicWhitespace(this.state.code[i].count) + "| "}
				{this.state.code[i].line}
			</p>);
		}
		//console.log(displayCode);

		return (
			<div
				style={{
					width: 500,
					height: 490,
					border: "thick solid #000000"
				}}
			>
				{displayCode}
			</div>
		)
	}
}

export default Pseudocode;