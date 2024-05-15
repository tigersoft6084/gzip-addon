const fs = require('fs');
const path = require('path');
const addon = require('./build/Release/gzipaddon');

// Read the gzipped file
const gzippedData = fs.readFileSync(path.join(__dirname, 'example.gz'));

// Create an ArrayBuffer from the gzipped data
const arrayBuffer = new Uint8Array(gzippedData).buffer;

// Call the unzip function
const result = addon.unzip(arrayBuffer);

// Print the result
fs.writeFileSync("result.txt", result);