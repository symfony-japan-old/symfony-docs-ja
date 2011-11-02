.. index::
   single: Tests

複数のクライアントのインタラクションをテストする方法
====================================================

チャットシステムのように異なるクライアント間のインタラクションをシミュレートする必要がある際には、次のようにクライアントを複数作成します。

::

    $harry = static::createClient();
    $sally = static::createClient();

    $harry->request('POST', '/say/sally/Hello');
    $sally->request('GET', '/messages');

    $this->assertEquals(201, $harry->getResponse()->getStatusCode());
    $this->assertRegExp('/Hello/', $sally->getResponse()->getContent());

コードがグローバルステートを維持するとき以外、もしくは、サードパーティのライブラリがグローバルステートのようなものを持っているとき以外は、動作します。このようなケースでは、クライアントを分離することができます。

::

    $harry = static::createClient();
    $sally = static::createClient();

    $harry->insulate();
    $sally->insulate();

    $harry->request('POST', '/say/sally/Hello');
    $sally->request('GET', '/messages');

    $this->assertEquals(201, $harry->getResponse()->getStatusCode());
    $this->assertRegExp('/Hello/', $sally->getResponse()->getContent());

透過的に分離されたクライアントは専用のクリーンな PHP のプロセスで実行されますので、副作用を避けることができます。

.. tip::

    クライアントの分離は遅いので、クライアントを１つメインプロセスに保持しておき、他のクライアントを分離しましょう。

.. 2011/11/02 ganchiku f869a3e48fdd8baafb2aa1859406d5893003bd82

