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
function polling(url, method, data) {
    method = method || 'get';
    data = data || null;
    const xhr = createXHR();
    xhr.onreadystatechange = function () {
        if (xhr.readyState == 4) {
            if (xhr.status >= 200 && xhr.status < 300 || xhr.status == 304) {
                console.log(xhr.responseText);
            } else {
                console.log("fail");
            }
        }
    };
    xhr.open(method, url, true);
    xhr.send(data);
}

setInterval(function () {
    polling('http://localhost:8088/time', 'get');
}, 5000);
