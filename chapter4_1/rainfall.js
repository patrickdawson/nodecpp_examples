"use strict";
const addon = require('./cpp/build/Release/rainfall');

let makeup = function makeup(max) {
    return Math.round(max * Math.random() * 100) / 100;
}

let locations = [];
for (let i=0; i<10; ++i) {
    const loc = {
        latitude: makeup(180),
        longitude: makeup(180),
        samples: [
            { date: "2015-07-20", rainfall: makeup(3) },
            { date: "2015-07-21", rainfall: makeup(3) },
            { date: "2015-07-22", rainfall: makeup(3) },
            { date: "2015-07-23", rainfall: makeup(3) }
        ]
    }
    locations.push(loc);
}

/*addon.calculate_results_sync(locations, results => {
    let i = 0;
    results.forEach(result => {
        console.log(`Result for Location ${i}`);
        console.log("--------------------------");
        console.log(`\tLatitude: ${locations[i].latitude.toFixed(2)}`);
        console.log(`\tLongitude: ${locations[i].longitude.toFixed(2)}`);
        console.log(`\tMean Rainfall: ${result.mean.toFixed(2)} cm`);
        console.log(`\tMedian Rainfall: ${result.median.toFixed(2)} cm`);
        console.log(`\tStandard Dev: ${result.standard_deviation.toFixed(2)} cm`);
        console.log(`\t Number Samples: ${result.n}`);
        i += 1;
    });
});*/

addon.calculate_results_async(locations, (err, results) => {
    let i = 0;
    results.forEach(result => {
        console.log(`Result for Location ${i}`);
        console.log("--------------------------");
        console.log(`\tLatitude: ${locations[i].latitude.toFixed(2)}`);
        console.log(`\tLongitude: ${locations[i].longitude.toFixed(2)}`);
        console.log(`\tMean Rainfall: ${result.mean.toFixed(2)} cm`);
        console.log(`\tMedian Rainfall: ${result.median.toFixed(2)} cm`);
        console.log(`\tStandard Dev: ${result.standard_deviation.toFixed(2)} cm`);
        console.log(`\t Number Samples: ${result.n}`);
        i += 1;
    });
});

console.log("Javascript program has completed.");
