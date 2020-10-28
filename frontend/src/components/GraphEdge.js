import React from 'react';

class GraphEdge extends React.Component {
	constructor(props) {
		super(props);
		this.state = {};
		/*this.start = start;
		this.end = end;
		this.weight = 0;
		this.reversed = false;
		this.greyedOut = false;
		this.color = "#000";
		this.thick = false;*/

		this.handleClick = this.handleClick.bind(this);
	}

	handleClick() {
		switch (this.props.question) {
			case null:
				console.log("clicking won't do much when it's just animating");
				break;
			case "dijkstra_updateNeighbors":
				break;
			case "dijkstra_highlightPath":
				this.props.addChange({
					edges: {
						// Dynamic keys
						[this.props.id]: { "color": "blue" }
					}
				});
				break;
			default:
				console.log("ERROR: unrecognized type of state " + this.props.question);
		}
	}

	render() {
		let arrowSize = 1.75;
		let pointsString = "0 0, " + arrowSize * 2 + " " + arrowSize + ", 0 " + arrowSize * 2;

		let color = "#000";
		if (this.props.changed != null) {
			if (this.props.changed.color != null) {
				color = this.props.changed.color;
			}
		}

		// Where to place the label for the edge's weight
		let weightPos = {
			x: (this.props.start.x + this.props.end.x) / 2 + 2,
			y: (this.props.start.y + this.props.end.y) / 2 - 2,
		};

		// Keep this consistent with what's in GraphNode.js's render function (radius + border + weird value that seems to be inversely related to the border size)
		const nodeRadius = 10 + 4 + 1 + arrowSize;
		// Calculate exactly where to end the line so the arrow isn't covered by the node
		let xDiff = this.props.end.x - this.props.start.x;
		let yDiff = this.props.end.y - this.props.start.y;
		let angle = Math.atan(yDiff / xDiff);
		let xOffset = nodeRadius * Math.cos(angle);
		let yOffset = nodeRadius * Math.sin(angle);
		let endX = this.props.end.x - (xOffset * (this.props.start.x <= this.props.end.x ? 1 : -1));
		let endY = this.props.end.y - (yOffset * (this.props.start.x <= this.props.end.x ? 1 : -1));

		return <g>
			<defs>
				<marker id="arrowhead" markerWidth="10" markerHeight="7"
					refX={arrowSize} refY={arrowSize} orient="auto">
					<polygon points={pointsString} />
				</marker>
			</defs>
			<line
				x1={this.props.start.x}
				y1={this.props.start.y}
				x2={endX}
				y2={endY}
				style={{
					stroke: color,
					strokeWidth: 3,
					cursor: "pointer",
				}}
				markerEnd="url(#arrowhead)"
				onClick={() => {
					this.handleClick()
				}
				}
			/>
			<text
				x={weightPos.x}
				y={weightPos.y}
				style={{
					font: "12px sans-serif",
				}}
				fill="#000"
			>
				{this.props.weight}
			</text>
		</g>
	}
}

export default GraphEdge;