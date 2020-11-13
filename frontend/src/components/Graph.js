import React from 'react';
import ReactDOM from 'react-dom';
import Doodle from './Doodle';
import '../styles/graph.css';
import GraphEdge from './GraphEdge';
import GraphNode from './GraphNode';
//import GraphGroup from './GraphGroup';

// The Graph component takes in a frame of data and renders the edges, nodes, and groups

class Graph extends React.Component {
	constructor(props) {
		super(props);
		this.state = {
			doodling: false,
			resetDoodles: this.props.resetDoodles, // Bool that toggles between true and false whenever it has to reset the user's doodles
			offset: { x: 0, y: 0 }, // Offset of the <svg> element on the screen
		};

		this.doodles = [];
		this.myRef = React.createRef();

		// Needed so these functions all know what "this" means
		this.handleMouse = this.handleMouse.bind(this);
		this.evalDoodle = this.evalDoodle.bind(this);
	}

	componentDidMount() {
		// maybe todo: this works fine like this, but can't handle scrolling at all...
		let rect = ReactDOM.findDOMNode(this).getBoundingClientRect();
		console.log(rect);
		this.setState({ offset: { x: rect["x"], y: rect["y"] } })
	}

	handleMouse(event) {
		// Possile design change: only allow during questions?
		/*if (this.props.question == null) {
			return;
		}*/
		let x = event.clientX - this.state.offset.x;
		let y = event.clientY - this.state.offset.y;
		switch (event.type) {
			case "mousemove":
				if (this.state.doodling) {
					let currentDoodle = this.doodles[this.doodles.length - 1]
					currentDoodle.points.push({
						x: x,
						y: y,
					});
					if (x < currentDoodle.borders.lowest.x) {
						currentDoodle.borders.lowest.x = x;
					}
					if (y < currentDoodle.borders.lowest.y) {
						currentDoodle.borders.lowest.y = y;
					}
					if (x > currentDoodle.borders.highest.x) {
						currentDoodle.borders.highest.x = x;
					}
					if (y > currentDoodle.borders.highest.y) {
						currentDoodle.borders.highest.y = y;
					}
				}
				break;
			case "mousedown":
				this.setState({ doodling: true });
				this.doodles.push({
					points: [{
						x: x,
						y: y,
					}],
					borders: {
						lowest: {
							x: Infinity,
							y: Infinity,
						},
						highest: {
							x: -Infinity,
							y: -Infinity,
						}
					}
				});
				break;
			case "mouseup":
				// It'd be cool to smooth these out one day and get rid of straight lines so they take up less points, right now these things are way too big of data pieces to be sent across the network so much
				let thisDoodle = this.doodles[this.doodles.length - 1];
				this.setState({ doodling: false });
				this.props.addDoodle(thisDoodle);
				this.evalDoodle(thisDoodle); // For clicker input
				break;
			default:
				console.log("unknown type: " + event.type);
		}
	}

	evalDoodle(doodle) {
		// Note: see handleMouse's mousedown handler for what the doodle object looks like
		if (doodle.points.length < 10) {
			// Don't bother with tiny accidental doodles
			return;
		}
		let b = doodle.borders;
		let change = {
			nodes: {}
		};
		for (const key in this.props.frame.nodes) {
			let node = this.props.frame.nodes[key];
			if (
				b.lowest.x <= node.position.x &&
				b.lowest.y <= node.position.y &&
				b.highest.x >= node.position.x &&
				b.highest.y >= node.position.y
			) {
				change.nodes[key] = { color: "blue" };
			}
		}
		// Send doodle as frontend change
		this.props.addChange(change);
	}

	componentDidUpdate() {
		// Reset doodles when parent deems it necessary
		if (this.state.resetDoodles !== this.props.resetDoodles) {
			this.doodles = [];
			this.setState({ resetDoodles: this.props.resetDoodles });
		}
	}

	render() {
		if (this.props.frame == null) {
			return null;
		}
		let edges = [];
		for (const key in this.props.frame.edges) {
			let edge = this.props.frame.edges[key];
			let start = this.props.frame.nodes[edge.start];
			let end = this.props.frame.nodes[edge.end];
			edges.push(<GraphEdge
				key={key}
				id={edge.id}
				start={{
					x: start.position.x,
					y: start.position.y,
				}} end={{
					x: end.position.x,
					y: end.position.y,
				}}
				weight={edge.weight}
				reversed={edge.reversed}
				greyedOut={edge.greyedOut}
				color={edge.color}
				thick={edge.thick}
				highlight={edge.highlight}

				question={this.props.question}
				addChange={this.props.addChange}
				changed={this.props.changes.edges != null ? this.props.changes.edges[key] : null}
			/>);
		}

		let nodes = [];
		for (const key in this.props.frame.nodes) {
			let node = this.props.frame.nodes[key];
			nodes.push(<GraphNode
				key={node.id}
				id={node.id}
				name={node.name}
				position={node.position}
				texture={node.texture}
				bold={node.bold}
				color={node.color}
				shape={node.shape}
				annotation={node.annotation}
				greyedOut={node.greyedOut}

				question={this.props.question}
				addChange={this.props.addChange}
				changed={this.props.changes.nodes != null ? this.props.changes.nodes[key] : null}
			/>);
		}

		let doodles = [];
		let count = 0;
		for (let i = 0; i < this.doodles.length; i++) {
			const doodle = this.doodles[i].points;
			doodles.push(<Doodle key={count} color="#000" points={doodle} />);
			count++;
		}
		for (let i = 0; i < this.props.otherDoodles.length; i++) {
			const doodle = this.props.otherDoodles[i].points;
			doodles.push(<Doodle key={count} color="#666" points={doodle} />);
			count++;
		}

		let stack = null;
		if (this.props.frame.stack != null) {
			let stackNodes = [];
			let boxDim = 50; // px height and width of box for stack elements
			let currX = this.props.width - boxDim
			let currY = this.props.height - boxDim;
			for (let i = this.props.frame.stack.length - 1; i >= 0; i--) {
				let stackNode = this.props.frame.stack[i];
				stackNodes.push(
					<g>
						<rect
							x={currX}
							y={currY}
							height={boxDim-20}
							width={boxDim}
							fill="none" stroke="#000" strokeWidth="2"
						></rect>
						<text
							x={currX + 7}
							y={currY + 19}
							fontFamily="Courier New"
						>{stackNode}</text>
					</g>
				);
				currY -= 30 + 3;
			}
			stack = <g>
				<text
					x={currX}
					y={currY + 19}
				>up next:</text>
				{stackNodes}
			</g>
		}

		// TODO: magnify SVG???

		return (
			<div
				id="graph-window"
				onMouseMove={this.handleMouse}
				onMouseDown={this.handleMouse}
				onMouseUp={this.handleMouse}
				margin="0px"
				padding="0px"
			>
				<svg
					id="graph-svg"
					ref="graph-svg"
					width={this.props.width}
					height={this.props.height}
				>
					{edges}
					{nodes}
					{doodles}
					{stack}
				</svg>
			</div>
		)
	}
}

export default Graph;