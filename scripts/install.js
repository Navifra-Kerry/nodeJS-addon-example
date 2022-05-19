const { exec } = require('child_process');

if (process.env.DEPLOY !== '1') {
    const child = exec('cmake-js compile');

    child.stdout.on('data', function (data) {
        process.stdout.write(data.toString());
    });

    //spit stderr to screen
    child.stderr.on('data', function (data) {
        process.stdout.write(data.toString());
    });
}
