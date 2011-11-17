.. index::
   single: Event Dispatcher

継承無しでメソッドの挙動をカスタマイズする方法
============================================================

メソッドの呼び出し前や後に何かを行う
---------------------------------------------

メソッドが呼ばれる直前、または呼ばれた直後に何かをしたい際には、メソッドの最初と最後でイベントをディスパッチすることができます。
::

    class Foo
    {
        // ...

        public function send($foo, $bar)
        {
            // do something before the method
            $event = new FilterBeforeSendEvent($foo, $bar);
            $this->dispatcher->dispatch('foo.pre_send', $event);

            // get $foo and $bar from the event, they may have been modified
            $foo = $event->getFoo();
            $bar = $event->getBar();
            // the real method implementation is here
            // $ret = ...;

            // do something after the method
            $event = new FilterSendReturnValue($ret);
            $this->dispatcher->dispatch('foo.post_send', $event);

            return $event->getReturnValue();
        }
    }

この例では、２つのイベントが投げられます。 ``foo.pre_send`` と ``foo.post_send`` です。 ``foo.pre_send`` は、メソッドが実行される前に、 ``foo.post_send`` はメソッドの実行後になります。どちらもカスタムイベントクラスを使用して、２つのイベントのリスナとコミュニケートします。これらのイベントクラスは、あなたによって作成される必要があり、この例では、リスナによって ``$foo``, ``$bar``, ``$ret`` 変数を取り出したり、セットできたりするようになってます。

例として ``FilterSendReturnValue`` が ``setReturnValue`` メソッドを持ってると想定すると、リスナはこのようになります。

.. code-block:: php

    public function onFooPostSend(FilterSendReturnValue $event)
    {
        $ret = $event->getReturnValue();
        // modify the original ``$ret`` value

        $event->setReturnValue($ret);
    }

.. 2011/11/18 ganchiku 4021613d0c9a5a967fc50ed68dacebc06833bd50

