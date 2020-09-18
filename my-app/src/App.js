import React from 'react';
import ReactDOM, { render } from 'react-dom';
import logo from './logo.svg';
import './App.css';

class NodeData {
  constructor(id, val) {
    this.id = id;
    this.val = val;
  }
}

class GraphData {
  constructor(ndCount=0) {
    this.ndList = [];
    for (let i = 0; i < ndCount; i++) {
      this.ndList.push(new NodeData(i, i));
    }
  }
  addNode(val) {

  }
}

class NodeDisplay extends React.Component {
  monkyNoise() {
    alert("OO OO AA AA");
  }
  render() {
    return (
      <span class="nodeCircle" onClick={this.monkyNoise}>
        <p class="nodeText">{this.props.val}</p>
      </span>
    );
  }
}

let granny = new GraphData(28);
class GraphDisplay extends React.Component {
  render() {
    let ndDispList = [] // could also use .map() here but I thought this would be more comprehensive.
    for (let i = 0; i < granny.ndList.length; i++) {
      let ndKey = "node_" + toString(granny.ndList[i].id);
      ndDispList.push(<NodeDisplay val={"monky # " + granny.ndList[i].val} i={ndKey}/>); 
    }
    return (
      <div>
        {ndDispList}
      </div>
    );
  }
}

function App() {
  return (
    <div className="App">
      <div className="nodeZone">
        <h2>WELCOME TO THE NODE ZONE</h2>
        <GraphDisplay/>
      </div>
    </div>
  );
}

export default App;