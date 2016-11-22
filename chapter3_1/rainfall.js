"use strict";
const addon = require('./cpp/build/Release/rainfall');

let location = {
    latitude: 40.71, longitude: -74.01,
    samples: [
        { date: "2016-06-07", rainfall: 2 },
        { date: "2016-08-12", rainfall: 2.1 },
        { date: "2016-08-12", rainfall: 1.8 },
        { date: "2016-08-12", rainfall: 1.9 }
    ]
};

let average = addon.avg_rainfall(location);
console.log("Average rain fall = " + average + " cm");

let data = addon.data_rainfall(location);
console.log("Mean = " + data.mean)
console.log("Median = " + data.median);
console.log("Standard Deviation = " + data.standard_deviation);
console.log("N = " + data.n);
