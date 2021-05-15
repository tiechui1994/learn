/**
 * 双向绑定 + 脏检查
 */
window.onload = function () {
  var scope = {
    increase: function () {
      this.data++;
    },
    decrease: function () {
      this.data--;
    },
    data: 0,
    faciend: 3
  };

  function Scope() {
    this.$$watchList = [];
  }

  Scope.prototype.$watch = function (name, getNewValue, listener) {
    var watch = {
      name: name,
      getNewValue: getNewValue,
      listener: listener || function () {
      }
    };

    this.$$watchList.push(watch);
  };

  Scope.prototype.$digest = function () {
    var dirty = true; //执行脏检查
    var checkTimes = 1; //脏检查的次数约束
    while (dirty) {
      dirty = this.$$digestOnce();
      checkTimes++;
      if (checkTimes > 10 && checkTimes) { // 脏检查次数的限制
        throw new Error('次数太多')
      }
    }
  };

  Scope.prototype.$$digestOnce = function () {
    var list = this.$$watchList;
    for (var i = 0; i < list.length; i++) {
      var watch = list[i];
      console.log(watch);
      var newValue = watch.getNewValue();
      var oldValue = watch.last;

      if (newValue !== oldValue) {
        watch.listener(newValue, oldValue); // 修改值
        dirty = true; // 再次检查修改后的值
      } else {
        dirty = false;
      }

      watch.last = newValue;
    }
    return dirty;
  };

  var $scope = new Scope();
  $scope.sum = 0;
  // 监听sum
  $scope.$watch('sum', function () {
    $scope.sum = scope.data * scope.faciend; // 计算sum的值
    return $scope[this.name]
  }, function (newValue, oldValue) {
    scope.sum = newValue;  // 将计算得到的值赋给sum
    console.log('sum:          newValue: ' + newValue + '~~~~' + ' oldValue: ' + oldValue);
  });

  function bind() { //绑定操作, ng-click事件的绑定
    var list = document.querySelectorAll('[ng-click]');
    for (var i = 0; i < list.length; i++) {
      list[i].onclick = (function (index) {
        return function () {
          var func = this.getAttribute('ng-click'); //事件函数名称
          console.log('ng-click: ' + func);
          scope[func](); //执行事件函数
          $scope.$digest(); //事件发生后的脏检查
          apply(); // 渲染数据
        }
      })(i);
    }
  }

  function apply() {
    var list = document.querySelectorAll('[ng-bind]');
    for (var i = 0; i < list.length; i++) {
      var bindData = list[i].getAttribute('ng-bind');
      console.log('属性:' + bindData + '为' + scope[bindData]);
      list[i].innerHTML = scope[bindData];
    }
  }

  bind(); // 绑定事件
  $scope.$digest(); // 首次脏检查
  apply(); //首次渲染数据
};