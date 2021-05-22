/*
    
*/

var Stats = function(){

    var startTime = Date.now(), prevTime = startTime;
    var ms = 0, msMin = Infinity, msMax = 0;
    var fps = 0, fpsMin = Infinity, fpsMax = 0;

    var container = document.createElement(' div ');
    container.id = "stats";
    container.addEventListener('mousedown', function(event) {
        event.preventDefault();
        setMode( ++mode % 2)
     }, false);
     container.style.cssText = 'width:80px; opacity:0.9;cursor: pointer';
    
     var fpsDiv = document.createElement('div');
     fpsDiv.id = 'fps';
     fpsDiv.style.cssText = 'padding: 0 0 3px 3px; text-align: left; background-color: #002';
     container.appendChild(fpsDiv);

     var fpsText = document.createElement('div');
     fpsText.id  = 'fpsText';
     fpxText.style.cssText = 'position:relative; width:74px; height:30px;background-color:#0ff'
     

     var fpsGraph = document.createElement('div');
     fpsGraph.id = 'fpsGraph';
     fpsGraph.style.cssText = 'position:relative; width: 74px; height:30px; background-color: #0f2';

     
}