import React from 'react';

class Doodle extends React.Component {
	constructor(props) {
		super(props);
		this.state = {
			points: [],
		};
	}

	render() {
		let path = "";
		for (let i = 0; i < this.props.points.length; i++) {
			path += this.props.points[i].x + "," + this.props.points[i].y + " ";
		}
		return <polyline points={path} fill="none" stroke={this.props.color} />
	}
}

export default Doodle;