Module.onRuntimeInitialized = () => {
    start();
};

var canvas = document.getElementById('mycanvas'); // get canvas element
var ctx = canvas.getContext('2d');
ctx.save();

const EMPTY = 0;
const WHITE = 1;
const BLACK = 2;
const UNCONSTRAINED = -1;
const ERROR = 0;
const UNSATISFIED = 1;
const SATISFIED = 2;

const FULL = 0;
const ORTHO = 1;
const FULL_EXCLUDE = 2;
const ORTHO_EXCLUDE = 3;



let cellSize;  // cellSize as a global variable
let g;  //  g as a global variable

const col2str = [" ", "□", "■"];
const num2str = [
    ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9"],     // empty
    ["🄋", "➀", "➁", "➂", "➃", "➄", "➅", "➆", "➇", "➈"],  // white
    ["⓿", "❶", "❷", "❸", "❹", "❺", "❻", "❼", "❽", "❾"],     // black
];

function start() {
    console.log("call start routine");
    g = Module._new_default(); 
    rendergame(g)
}

function square2str(n, c) {
    if (n === UNCONSTRAINED) {
        return col2str[c];
    } else {
        return num2str[c][n];
    }
}

canvas.addEventListener('click', canvasLeftClick); // left click event

function canvasLeftClick(event) {
    const x = event.offsetX;
    const y = event.offsetY;
    const col = Math.floor(x / cellSize);
    const row = Math.floor(y / cellSize);
    const color = Module._get_color(g, row, col); // Get the color at the clicked cell

    switch (color) {
        case EMPTY:
            Module._play_move(g, row, col, WHITE); // Set to WHITE
            break;
        case WHITE:
            Module._play_move(g, row, col, BLACK); // Set to BLACK
            break;
        case BLACK:
            Module._play_move(g, row, col, EMPTY); // Set to EMPTY
            break;
        default:
            break;
    }

    rendergame(g);  // Re-render the game grid
}

/* ******************** canvas drawing ******************** */

function rendergame(g) {
    const rows = Module._nb_rows(g);
    const cols = Module._nb_cols(g);
    const gridSize = rows;
    cellSize = canvas.width / gridSize;  // Calculate cellSize based on canvas width and grid size

    ctx.clearRect(0, 0, canvas.width, canvas.height); // Clear the canvas

    ctx.font = '24px Arial';  // set font size

    for (let row = 0; row < rows; row++) {
        for (let col = 0; col < cols; col++) {
            const constraint = Module._get_constraint(g, row, col); // Get the constraint at (row, col)
            const text = constraint === UNCONSTRAINED ? ' ' : constraint.toString(); // Convert constraint to string, replace -1 with empty space
            const textX = col * cellSize + cellSize / 2 - 8; // X coordinate for the text,centering
            const textY = row * cellSize + cellSize / 2 + 8; // Y coordinate for the text, centering
            const color = Module._get_color(g, row, col); // Get the color at (row, col)
            const status = Module._get_status(g, row, col); // Get the status at (row, col)

            if (color === EMPTY) {
                ctx.fillStyle = 'rgb(115, 155, 198)'; //lightblue
            } else if (color === WHITE) {
                ctx.fillStyle = 'white';  // White
            } else if (color === BLACK) {
                ctx.fillStyle = 'black'; // Black
            }

            ctx.fillRect(col * cellSize, row * cellSize, cellSize, cellSize);

            if (status === ERROR) {
                ctx.fillStyle = 'red'; // Red for error status
            } else if (status === UNSATISFIED) {
                ctx.fillStyle = 'rgb(65, 93, 123)'; 
            } else {
                ctx.fillStyle = 'cyan';  // Blue for satisfied status
            }

            ctx.fillText(text, textX, textY);
        }
    }

    ctx.strokeStyle = 'black';

    for (let x = 0; x <= canvas.width; x += cellSize) {
        ctx.moveTo(x, 0);
        ctx.lineTo(x, canvas.height);
        ctx.stroke();
    }

    for (let y = 0; y <= canvas.height; y += cellSize) {
        ctx.moveTo(0, y);
        ctx.lineTo(canvas.width, y);
        ctx.stroke();
    }
}
//handling the buttons click events
document.getElementById('undo').addEventListener('click', undo);
document.getElementById('redo').addEventListener('click', redo);
document.getElementById('restart').addEventListener('click', restart);
document.getElementById('solve').addEventListener('click', solve);
document.getElementById('random').addEventListener('click', random);

function undo() {
    Module._undo(g);
    rendergame(g);
}

function redo() {
    Module._redo(g);
    rendergame(g);
}

function restart() {
    Module._restart(g);
    rendergame(g);
}

function solve() {
    Module._solve(g);
    rendergame(g);
}

function random() {
    g=Module._new_random(5, 5, true, FULL, false, 0.4, 0.5);
    rendergame(g);
    console.error("Error executing random function:", error);
}