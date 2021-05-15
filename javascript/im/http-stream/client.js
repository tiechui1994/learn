function createXHR() {
    if (typeof XMLHttpRequest != 'undefined') {
        return new XMLHttpRequest();
    } else if (typeof ActiveXObject != 'undefined') {
        // 兼容IE低版本考虑
        if (typeof arguments.callee.activeXString != "string") {
            let versions = ["MSXML2.XMLHttp.6.0", "MSXML2.XMLHttp.3.0", "MSXML2.XMLHttp"], i, len;
            for (i = 0, len = versions.length; i < len; i++) {
                try {
                    new ActiveXObject(versions[i]);
                    arguments.callee.activeXString = versions[i];
                    break;
                } catch (ex) {
                }
            }
        }
        return new ActiveXObject(arguments.callee.activeXString);
    } else {
        throw new Error("no xhr object available");
    }
}

function createStreamClient(url, handle, done) {
    const xhr = createXHR();
    let received = 0;
    xhr.onreadystatechange = function () {
        let result;
        if (xhr.readyState == 3) {
            result = xhr.responseText.substring(received);
            received += result.length;
            handle(result)
        } else if (xhr.readyState == 4) {
            done(xhr.responseText)
        }
    };

    console.log('start a request');
    xhr.open('get', url, true);
    xhr.send(null);

    return xhr;
}

createStreamClient('http://localhost:8088/stream', function (data) {
    console.log(data)
}, function (data) {
    console.log('Done, ' + data)
});

