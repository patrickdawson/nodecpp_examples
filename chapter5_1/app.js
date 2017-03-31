"use strict";
const addon = require('./cpp/build/Release/polynomial');

const poly = new addon.Polynomial(1, 3, 2);
console.log(poly);