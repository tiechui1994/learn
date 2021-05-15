/**
 * 脏检查操作
 */

window.onload = function () {
  // 定义一个页面的scope,存储的是model
  function $scope() {
    this.$$watchList = []
  }

  //定义$watch方法, 注册监听器
  $scope.prototype.$watch = function (name, getNewValue, listener) {
    var watch = {
      name: name,
      getNewValue: getNewValue,
      listener: listener
    };

    this.$$watchList.push(watch)
  };

  //监听操作
  $scope.prototype.$digest = function () {
    var list = this.$$watchList;
    var dirty = true; //执行脏检查
    var checkTimes = 1; //脏检查的次数约束
    while (dirty) {
      dirty = false;
      for (var i = 0; i < list.length; i++) {
        var watch = list[i];
        console.log(watch);
        var newValue = watch.getNewValue();
        var oldValue = watch.last;

        if (newValue !== oldValue) {
          watch.listener(newValue, oldValue); // 修改值
          dirty = true; // 再次检查修改后的值
        }
      }

      checkTimes++;
      if (checkTimes > 10 && checkTimes) { // 脏检查次数的限制
        throw new Error('次数太多')
      }
    }
  };

  // 实例化操作
  var scope = new $scope();
  scope.first = 0;
  scope.second = 0;
  scope.$watch('first', function () {
    console.log('I am first');
    return scope[this.name];
  }, function (newValue, oldValue) {
    scope.second++;
    console.log('newValue: ' + newValue + ' ~~~~~~~~~~' + ' oldValue: ' + oldValue)
  });

  scope.$watch('second', function () {
    console.log('I am second');
    return scope[this.name];
  }, function (newValue, oldValue) {
    scope.first++;
    console.log('newValue: ' + newValue + ' ~~~~~~~~~~' + ' oldValue: ' + oldValue)
  });

  scope.$digest()
};
