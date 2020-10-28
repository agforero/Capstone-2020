import React from 'react';

//import Textures from '../constants/Textures';
//import Shapes from '../constants/Shapes';


class GraphNode extends React.Component {
	constructor(props) {
		super(props);
		this.state = {};
		/*this.id = id;
		this.name = name;
		this.position = { x: x, y: y };
		this.texture = Textures.normal;
		this.color = "#fff";
		this.shape = Shapes.circle;
		this.annotation = "";
		this.greyedOut = false;*/

		this.handleClick = this.handleClick.bind(this);
	}

	handleClick() {
		switch (this.props.question) {
			case null:
				console.log("clicking won't do much when it's just animating");
				break;
			case "dijkstra_updateNeighbors":
				let newVal = prompt("Update in PQ", this.props.annotation);
				console.log(newVal);
				if (newVal !== "" && newVal !== null) {
					this.props.addChange({
						nodes: {
							// Dynamic keys
							[this.props.id]: { "annotation": newVal }
						}
					});
				} else {
					// TODO: this.props.addChange() whatever it sent over last time to undo the change
				}
				break;
			case "dijkstra_highlightPath":
				break;
			default:
				console.log("ERROR: unrecognized type of state " + this.props.question);
		}
	}

	render() {
		// Overwrite default things in the animation when changes have been made
		let color = this.props.color;
		let annotation = this.props.annotation;
		if (this.props.changed != null) {
			if (this.props.changed.color != null) {
				color = this.props.changed.color;
			}
			if (this.props.changed.annotation != null) {
				annotation = this.props.changed.annotation;
			}
		}

		return <g>
			<circle
				cx={this.props.position.x}
				cy={this.props.position.y}
				stroke={color}
				r="10"
				strokeWidth="4"
				fill="white"
			/>
			<text
				x={this.props.position.x - 4}
				y={this.props.position.y + 5}
				style={{
					font: "12px sans-serif",
					fontWeight: "bold",
				}}
				fill="#000"
			>
				{this.props.name}
			</text>
			<text
				x={this.props.position.x + 6}
				y={this.props.position.y - 10}
				fill={annotation === this.props.annotation ? this.props.color : "#00f"}
				onClick={() => { this.handleClick() }}
				style={{
					font: "12px sans-serif",
					cursor: "pointer",
				}}
			>
				{annotation}
			</text>
		</g>
	}
}

export default GraphNode;