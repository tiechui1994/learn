function longConnecting() {
    window.setInterval(function () {
       console.log($($('#frame').get(0).contentDocument).find("body").text());

        $('#frame').attr("src", "http://locolhost:8088/connect");

        window.setTimeout(function () {
            window.frames["polling"].location.reload()
        }, 1000)
    }, 5000)
}

console.log('start a long connect');
longConnecting();