// constants
const COLOR_BG = "black";
const COLOR_CUBE = "yellow";
const COLOR_GROUND_PLANE = "grey";
const SPEED_X = 0.05; // rps
const SPEED_Y = 0.15; // rps
const SPEED_Z = 0.10; // rps
const POINT3D = function(x, y, z) { this.x = x; this.y = y; this.z = z; };

// set up the canvas and context
var canvas = document.getElementById("visualization");
var ctx = canvas.getContext("2d");

// dimensions
var h = 500; //display area should be square
var w = 500;
canvas.height = h;
canvas.width = w;

// colours and lines
ctx.fillStyle = COLOR_BG;
ctx.lineWidth = w / 100;
ctx.lineCap = "round";

// cube parameters
var box_cx = w / 2;
var box_cy = h / 2;
var box_cz = h / 4;

var boxSizeX = h / 8;
var boxSizeY = h / 16;
var boxSizeZ = h / 64;
var boxVertices = [
    new POINT3D(box_cx - boxSizeX, box_cy - boxSizeY, box_cz - boxSizeZ),
    new POINT3D(box_cx + boxSizeX, box_cy - boxSizeY, box_cz - boxSizeZ),
    new POINT3D(box_cx + boxSizeX, box_cy + boxSizeY, box_cz - boxSizeZ),
    new POINT3D(box_cx - boxSizeX, box_cy + boxSizeY, box_cz - boxSizeZ),
    new POINT3D(box_cx - boxSizeX, box_cy - boxSizeY, box_cz + boxSizeZ),
    new POINT3D(box_cx + boxSizeX, box_cy - boxSizeY, box_cz + boxSizeZ),
    new POINT3D(box_cx + boxSizeX, box_cy + boxSizeY, box_cz + boxSizeZ),
    new POINT3D(box_cx - boxSizeX, box_cy + boxSizeY, box_cz + boxSizeZ)
];
var boxEdges = [
    [0, 1], [1, 2], [2, 3], [3, 0], // back face
    [4, 5], [5, 6], [6, 7], [7, 4], // front face
    [0, 4], [1, 5], [2, 6], [3, 7] // connecting sides
];

//camera parameters
var camPosX = 0;
var camPosY = w/1.4;
var camPosZ = w/1.4;
var camPosPitch = 65.0;
var camPosRoll = 0.0;
var camPosYaw = 45.0;


// ground plane parameters
var gp_cx = w / 2;
var gp_cy = h / 2;
var gp_cz = 0;
var groundPlaneSize = h / 3;

var groundPlaneVerticies = [
    new POINT3D(gp_cx - groundPlaneSize, gp_cy - groundPlaneSize, gp_cz),
    new POINT3D(gp_cx + groundPlaneSize, gp_cy - groundPlaneSize, gp_cz),
    new POINT3D(gp_cx + groundPlaneSize, gp_cy + groundPlaneSize, gp_cz),
    new POINT3D(gp_cx - groundPlaneSize, gp_cy + groundPlaneSize, gp_cz)
];
var groundPlaneEdges = [
    [0, 1], [1, 2], [2, 3], [3, 0]
];


// imu axis
var axis_cx = box_cx;
var axis_cy = box_cy;
var axis_cz = box_cz;
var axis_size = h / 12;

var axisVerticies = [
    new POINT3D(axis_cx, axis_cy, axis_cz),
    new POINT3D(axis_cx + axis_size, axis_cy, axis_cz),
    new POINT3D(axis_cx, axis_cy - axis_size, axis_cz),
    new POINT3D(axis_cx, axis_cy, axis_cz + axis_size),
];
var axisEdges = [
    [0, 1], [0, 2], [0, 3]
];


// ground plane axis
var gpAxis_cx = gp_cx;
var gpAxis_cy = gp_cy;
var gpAxis_cz = gp_cz;
var gpAxis_size = h / 12;

var gpAxisVerticies = [
    new POINT3D(gpAxis_cx, gpAxis_cy, gpAxis_cz),
    new POINT3D(gpAxis_cx + gpAxis_size, gpAxis_cy, gpAxis_cz),
    new POINT3D(gpAxis_cx, gpAxis_cy - gpAxis_size, gpAxis_cz),
    new POINT3D(gpAxis_cx, gpAxis_cy, gpAxis_cz + gpAxis_size),
];
var gpAxisEdges = [
    [0, 1], [0, 2], [0, 3]
];

// set up the animation loop
var timeDelta, timeLast = 0;
requestAnimationFrame(loop);

//Input variables
var pitchDeg = 0;
var rollDeg = 0;
var yawDeg = 0;

function setPitchDeg(angle_in){
    pitchDeg = angle_in;
}

function setRollDeg(angle_in){
    rollDeg = angle_in;
}

function setYawDeg(angle_in){
    yawDeg = angle_in;
}


function loop(time_ms) {

    // calculate the time difference
    timeDelta = time_ms - timeLast;
    timeLast = time_ms;

    //temp test - rotate 30 deg/sec
    yawDeg += 30.0 * timeDelta / 1000.0;
    //rollDeg += 40.0 * timeDelta / 1000.0;
    //pitchDeg += 73.0 * timeDelta / 1000.0;


    // background
    ctx.fillRect(0, 0, w, h);

    boxDrawVerticies = structuredClone(boxVertices);
    boxAxisDrawVerticies = structuredClone(axisVerticies);
    groundPlaneDrawVerticies = structuredClone(groundPlaneVerticies);
    gpAxisDrawVerticies = structuredClone(gpAxisVerticies);

    //Apply rotation to the IMU representation
    rotate(boxDrawVerticies, rollDeg, -1.0*pitchDeg, -1.0*yawDeg, box_cx, box_cy, box_cz, 0, 0, 0);
    rotate(boxAxisDrawVerticies, rollDeg, -1.0*pitchDeg, -1.0*yawDeg, axis_cx, axis_cy, axis_cz, 0, 0, 0);

    //Apply camera/perspective
    rotate(boxDrawVerticies, camPosPitch, camPosRoll, camPosYaw, camPosX, camPosY, camPosZ, camPosX, camPosY, camPosZ);
    rotate(boxAxisDrawVerticies, camPosPitch, camPosRoll, camPosYaw, camPosX, camPosY, camPosZ, camPosX, camPosY, camPosZ);
    rotate(groundPlaneDrawVerticies, camPosPitch, camPosRoll, camPosYaw, camPosX, camPosY, camPosZ, camPosX, camPosY, camPosZ);
    rotate(gpAxisDrawVerticies, camPosPitch, camPosRoll, camPosYaw, camPosX, camPosY, camPosZ, camPosX, camPosY, camPosZ);

    // draw each edge
    drawObj(groundPlaneEdges, groundPlaneDrawVerticies, COLOR_GROUND_PLANE);
    drawObj(gpAxisEdges, gpAxisDrawVerticies, null);
    drawObj(boxEdges, boxDrawVerticies, COLOR_CUBE);
    drawObj(axisEdges, boxAxisDrawVerticies, null);

    // call the next frame
    requestAnimationFrame(loop);
}

function drawObj(edges, verticies, color){
    if(color != null){
        ctx.strokeStyle = color;
    }

    colorIdx = 0;

    for (let edge of edges) {

        if(color == null){
            if(colorIdx == 0){
                ctx.strokeStyle = "red";
            } else if (colorIdx == 1){
                ctx.strokeStyle = "green";
            }else if (colorIdx == 2){
                ctx.strokeStyle = "blue";
            }
        }

        ctx.beginPath();
        ctx.moveTo(verticies[edge[0]].x, verticies[edge[0]].y);
        ctx.lineTo(verticies[edge[1]].x, verticies[edge[1]].y);
        ctx.stroke();

        colorIdx++;
    }
}

function rotate(verticies, pitch, roll, yaw, cx, cy, cz, offsetx, offsety, offsetz){
        // rotate  along the z axis
    //let angle = timeDelta * 0.001 * SPEED_Z * Math.PI * 2;
    angle = yaw * Math.PI / 180.0;
    for (var vertIdx = 0; vertIdx < (verticies.length) ; vertIdx++) {
        let dx = verticies[vertIdx].x - cx;
        let dy = verticies[vertIdx].y - cy;
        let x = dx * Math.cos(angle) - dy * Math.sin(angle);
        let y = dx * Math.sin(angle) + dy * Math.cos(angle);
        verticies[vertIdx].x = x + cx;
        verticies[vertIdx].y = y + cy;
    }

    // rotate along the x axis
    angle = pitch * Math.PI / 180.0;
    for (var vertIdx = 0; vertIdx < (verticies.length) ; vertIdx++) {
        let dy = verticies[vertIdx].y - cy;
        let dz = verticies[vertIdx].z - cz;
        let y = dy * Math.cos(angle) - dz * Math.sin(angle);
        let z = dy * Math.sin(angle) + dz * Math.cos(angle);
        verticies[vertIdx].y = y + cy;
        verticies[vertIdx].z = z + cz;
    }

    // rotate along the y axis
    angle = roll * Math.PI / 180.0;
    for (var vertIdx = 0; vertIdx < (verticies.length) ; vertIdx++) {
        let dx = verticies[vertIdx].x - cx;
        let dz = verticies[vertIdx].z - cz;
        let x = dz * Math.sin(angle) + dx * Math.cos(angle);
        let z = dz * Math.cos(angle) - dx * Math.sin(angle);
        verticies[vertIdx].x = x + cx;
        verticies[vertIdx].z = z + cz;
    }

    //Position Offset
    for (var vertIdx = 0; vertIdx < (verticies.length) ; vertIdx++) {
        verticies[vertIdx].x -= offsetx;
        verticies[vertIdx].y -= offsety;
        verticies[vertIdx].z -= offsetz;
    }

}