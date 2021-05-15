const fs = require('fs');
const http = require('http');
let count = 0; // 统计发送的数据量

http.createServer(function (req, res) {
    if (req.url == '/stream') {
        res.writeHead(200, {
            'content-type': 'multipart/octet-stream', // 流式结果
            'Access-Control-Allow-Origin':'*' // 同源问题
        });

        let timer = setInterval(function () {
            sendRandomData(timer, res)
        }, 1000)
    }

    if (req.url == '/') {
        fs.readFile('./client.html', 'binary', function (err, file) {
            if (!err) {
                res.writeHead(200, {
                    'Content-Type': 'text/plain',
                    'Access-Control-Allow-Origin':'*'
                });
                res.write(file, 'binary');
                res.end()
            }
        })

    }
}).listen(8088, 'localhost');

function sendRandomData(timer, res) {
    let randomNum = Math.floor(1000*Math.random());
    res.write(randomNum.toString());
    console.log(randomNum);
    if (++count == 10000) { // 发送的数据量
        clearInterval(timer);
        res.end(randomNum.toString())
    }
}
