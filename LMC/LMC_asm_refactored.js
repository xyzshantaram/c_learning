const fs = require('fs');
const { stringify } = require('querystring');
/* To be ported to C once I get the logic down. */

Array.prototype.remove = function (v) {
    if (this.indexOf(v) != -1) {
        this.splice(this.indexOf(v), 1);
        return true;
    }
    return false;
}

String.prototype.replaceAll = function (arg1, arg2) {
    let toRet = this;
    while (toRet.match(arg1)) {
        toRet = toRet.replace(arg1, arg2);
    }
    return toRet;
}

let args = process.argv.slice(2);
if (args.length < 2) {
    console.log("too few arguments...");
}

let label_addresses = {}
let opcodes = ["HLT", "ADD", "SUB", "STA", "LDA", "BRA", "BRZ", "BRP", "INP", "OUT", "DAT"];

let file = fs.readFileSync(args[0]);
file = file.toString().replaceAll(/\/\/(.*)/, ' '); // strip comments
file = file.replaceAll(/  +/g, ' '); // strip extra whitespace
file = file.replaceAll("\n ", '\n'); // strip whitespace at the starting of a line
let file_lines = file.split('\n');

let line_no = 0;
let new_lines = [];

for (let raw_line of file_lines) {
    let x = raw_line.trim();
    if (x === "" || x.startsWith("//")) {
        continue;
    }
    
    let tokens = x.split(' ');

    if (tokens.length > 3) {
        console.error("");
        process.exit(1);
    }
    
    for (let y of tokens) {
        y = y.trim();
        if (y === "") {
            tokens.remove(y);
        }
    }

    if (!opcodes.includes(tokens[0])) {
        if (!Object.keys(label_addresses).includes(tokens[0])) {
            label_addresses[tokens[0]] = line_no;
            tokens.remove(tokens[0]);
        }
    }
    if (tokens.length == 1) {
        tokens[1] = "0";
    }
    line_no += 1;
    new_lines.push(tokens.join(' '));
}

let final = []

for (let x of new_lines) {
    let tokens = x.split(' ');
    for (let x of Object.keys(label_addresses)) {
        let pos = tokens.indexOf(x);
        if (pos != -1) {
            tokens[pos] = label_addresses[x];
        }
    }
    final.push(tokens.join(' '));
}

function array_contains_string(arr, str) {
    let index = 0;
    for (let x of arr) {
        if (str === (x)) {
            return index;
        }
        index += 1;
    }
    return -1;
}

let final_str = final.join("\n");
console.log(final_str);

fs.writeFileSync(args[1], final_str);
