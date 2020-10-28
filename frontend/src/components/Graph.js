import React from 'react';
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
			resetDoodles: this.props.resetDoodles,
		};

		this.doodles = []; // TODO: bool from parent to reset doodles?

		// Needed so these functions all know what "this" means
		this.handleMouse = this.handleMouse.bind(this);
		this.evalDoodle = this.evalDoodle.bind(this);
	}

	handleMouse(event) {
		// TODO: only allow during questions??
		/*if (this.props.question == null) {
			return;
		}*/
		let x = event.clientX;
		let y = event.clientY;
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
		// TODO: figure out how to reset doodles
		/*if (this.props.question == null) {
			this.doodles = [];
		}*/
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

		return (
			<div
				onMouseMove={this.handleMouse}
				onMouseDown={this.handleMouse}
				onMouseUp={this.handleMouse}
				margin="0px"
				padding="0px"
			>
				<svg width="500" height="300">
					{edges}
					{nodes}
					{doodles}
				</svg>
			</div>
		)
	}
}

export default Graph;