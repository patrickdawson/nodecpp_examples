"use strict";
const addon = require('./cpp/build/Release/rainfall');

let location = {
    latitude: 40.71, longitude: -74.01,
    samples: [
        { date: "2016-06-07", rainfall: 2 },
        { date: "2016-08-12", rainfall: 0.5 }
    ]
};
let average = addon.avg_rainfall(location);

console.log("Average rain fall = " + average + " cm");
