"use strict";
const Polynomial = require('./cpp/build/Release/polynomial').Polynomial;

var poly = new Polynomial(1, 3, 2);
console.log(poly.at(4)) // prints 30
poly.c = 0;
console.log(poly.at(4)) // prints 28
console.log(poly); // Polynomial {c:0}