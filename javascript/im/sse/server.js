const fs = require('fs');
const http = require('http');
let count = 0;

http.createServer(function (req, res) {
    if (req.url == '/event') {
        res.writeHead(200, {
            "Content-Type": "text/event-stream", // sse的特有格式
            "Cache-Control": "no-cache",
            'Access-Control-Allow-Origin': '*', // 跨越
            "Connection": "keep-alive" // 保持连接
        });
        let timer = setInterval(function () {
            if (++count == 100) {
                clearInterval(timer);
                res.end()
            } else {
                res.write('id: ' + count + '\n');
                res.write('data: ' + new Date().toLocaleString() + '\n\n')
            }
        }, 1000)
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
