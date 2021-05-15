let sourceEvent = new EventSource('http://localhost:8088/event');

sourceEvent.addEventListener('message', function (e) {
   console.log(e.data)
}, false);

sourceEvent.onopen = function () {
    console.log('start a request')
};

sourceEvent.onerror = function (error) {
  console.log(error)
};