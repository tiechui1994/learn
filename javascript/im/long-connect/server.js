const fs = require('fs');
const http = require('http');

http.createServer(function (req, res) {
    if (req.url == '/connect') {
        setInterval(function () {
            sendData(res);
        }, 5000)
    }

    if (req.url == '/') {
        fs.readFile('./client.html', 'binary', function (err, file) {
            if (!err) {
                res.writeHead(200, {
                    'Content-Type': 'text/html',
                    'Access-Control-Allow-Origin': '*'
                });
                res.write(file, 'binary');
                res.end()
            }
        })
    }
}).listen(8088, 'localhost');


function sendData(res) {
    let randomNum = Math.floor(100*Math.random());
    console.log(randomNum);

    if (randomNum>=0 && randomNum <=10) {
        res.writeHead(200, {
            'Content-Type': 'text/html',
            'Access-Control-Allow-Origin':'*'
        });
        res.end(new Date().toLocaleString())
    }
}
