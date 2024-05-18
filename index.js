const fs = require('fs');
const path = require('path');
const zlib = require('zlib');
const addon = require('./build/Release/gzipaddon');

// Read the gzipped file
const gzippedData = fs.readFileSync(path.join(__dirname, 'example.gz'));

// Create an ArrayBuffer from the gzipped data
console.time('Creating Buffer');
const arrayBuffer = new Uint8Array(gzippedData).buffer;
console.timeEnd('Creating Buffer');



// Print the result from the C++ addon

// Measure time for Node.js zlib
console.time('Node.js zlib');
zlib.gunzip(gzippedData, (err, resultZlib) => {
  if (err) {
    console.error('Error with zlib:', err);
  } else {
    console.timeEnd('Node.js zlib');
    // Print the result from Node.js zlib
  }
});

// Measure time for the C++ addon
console.time('C++ addon');
const resultAddon = addon.unzip(arrayBuffer);
console.timeEnd('C++ addon');