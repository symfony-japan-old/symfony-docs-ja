.. 2011/07/23 hidenorigoto 4690cac4dcc3fb39fa9a5dff0bdf66726441c177

.. index::
   single: Internals

内部構造
========

Symfony2 内部の動作や拡張方法について、さらに知りたいと思ったのではないでしょうか。
この章では、Symfony2 の内部構造の詳細について説明します。

.. note::

    Symfony2 が背後でどのように動作しているのかを知りたい場合や、Symfony2 自体を拡張したいといった場合に、この章を読んでください。

概観
----

Symfony2 のコードは、いくつかの独立したレイヤーで構成されています。
以降で説明する各レイヤーは、1 つ手前のレイヤーの上に位置づけられます。

.. tip::

    オートロードは、フレームワークでは直接管理されていません。
    フレームワークとは独立し、\ :class:`Symfony\\Component\\ClassLoader\\UniversalClassLoader` クラスと ``src/autoload.php`` ファイルを使ってオートロードが行われます。
    オートロードに関する詳細は、\ :doc:`オートローダーの章 </cookbook/tools/autoloader>`\ を参照してください。

``HttpFoundation`` コンポーネント
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

最も最下層に位置するのが、\ :namespace:`Symfony\\Component\\HttpFoundation` コンポーネントです。
HttpFoundation には、HTTP を扱うのに必要なオブジェクトがあります。
これらは、PHP のネイティブ関数や変数のいくつかをオブジェクト指向で抽象化したものです。

* :class:`Symfony\\Component\\HttpFoundation\\Request` クラスは、PHP の主要なグローバル変数である ``$_GET``\ 、\ ``$_POST``\ 、\ ``$_COOKIE``\ 、\ ``$_FILES``\ 、\ ``$_SERVER`` を抽象化しています。

* :class:`Symfony\\Component\\HttpFoundation\\Response` クラスは、\ ``header()`` \、\ ``setcookie()``\ 、\ ``echo`` などの PHP 関数を抽象化しています。

* :class:`Symfony\\Component\\HttpFoundation\\Session` クラスと :class:`Symfony\\Component\\HttpFoundation\\SessionStorage\\SessionStorageInterface` インタフェースは、セッションを管理するための ``session_*()`` 関数などを抽象化しています。

``HttpKernel`` コンポーネント
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

HttpFoundation の上のレイヤーは、\ :namespace:`Symfony\\Component\\HttpKernel` コンポーネントです。
HttpKernel HTTP の動的な機能を担当します。
Request クラスや Response クラスの薄いラッパーで、リクエストの標準的な処理方法を提供します。
また、オーバーヘッドの少ない Web フレームワークを作成するための、拡張ポイントやツールもこのレイヤーで提供されています。

また、依存性注入 (Dependency Injection) コンポーネントや強力なプラグインシステム (バンドル) もこのレイヤーで提供されており、設定や拡張を柔軟に行なえます。

.. seealso::

    :doc:`HttpKernel コンポーネントに関する詳細 <kernel>`\ 、
    :doc:`依存性注入 (Dependency Injection)</book/service_container>`\ 、
    :doc:`バンドルについて </cookbook/bundles/best_practices>`

``FrameworkBundle`` バンドル
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

:namespace:`Symfony\\Bundle\\FrameworkBundle` バンドルは、主要なコンポーネントやライブラリを結びつけ、軽量で高速な MVC フレームワークとしてまとめあげたものです。
デフォルトの設定や規約は実用的なので、とてもスムーズに習得することができます。

.. index::
   single: Internals; Kernel

カーネル
--------

:class:`Symfony\\Component\\HttpKernel\\HttpKernel` クラスは Symfony2 の中心となるクラスで、クライアントからのリクエストの処理を担当します。
この処理の主な内容は、\ :class:`Symfony\\Component\\HttpFoundation\\Request` オブジェクトをを受け取り、\ :class:`Symfony\\Component\\HttpFoundation\\Response` オブジェクトに変換することです。

Symfony2 のカーネルクラスは、\ :class:`Symfony\\Component\\HttpKernel\\HttpKernelInterface` インターフェイスを実装する必要があります。

::

    function handle(Request $request, $type = self::MASTER_REQUEST, $catch = true)

.. index::
   single: Internals; Controller Resolver

コントローラ
~~~~~~~~~~~~

Request を Response に変換するために、カーネルは "コントローラ" を利用します。
コントローラには、PHP の関数としてコール可能な任意の構造を利用できます。

カーネルは、実行するべきコントローラを見つける処理を、\ :class:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface` の実装クラスへ委譲します。

::

    public function getController(Request $request);

    public function getArguments(Request $request, $controller);

:method:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface::getController` メソッドは、特定の Request オブジェクトに対応するコントローラ (呼び出し可能な PHP コード) を返します。
このメソッドのデフォルトの実装 (:class:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolver`)\ では、リクエストの属性に設定された \ ``_controller``\ を元にコントローラを特定します。この属性には、``Bundle\BlogBundle\PostController:indexAction`` のように "クラス::メソッド" という形式の文字列で、コントローラ名が格納されています。
).

.. tip::

    デフォルトの実装では、\ :class:`Symfony\\Bundle\\FrameworkBundle\\EventListener\\RouterListener`\ を使って Request オブジェクトの ``_controller`` 属性を定義しています (:ref:`kernel-core-request` も参照してください)。

.. TODO 以下の部分の翻訳をブラッシュアップ

:method:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface::getArguments` メソッドは、コール可能な PHP コードであるコントローラへ渡す、引数の配列を返します。
このメソッドのデフォルトの実装では、Request オブジェクトの属性を基に、対象となるメソッドの引数が自動的に解決されます。

.. sidebar:: コントローラメソッドの引数と Request オブジェクトの属性のマッチング

    メソッドのそれぞれの引数に対して、Request オブジェクトで同じ名前の属性が取得されます。
    同じ名前の属性が Request オブジェクトに定義されておらず、かつ引数のデフォルト値が定義されている場合は、デフォルト値が使われます。

    ::

        // Symfony2 により、必須の 'id' 属性と、
        // 任意の 'admin' 属性が Request オブジェクトから取得される。
        public function showAction($id, $admin = true)
        {
            // ...
        }

.. index::
  single: Internals; Request Handling

リクエストの処理
~~~~~~~~~~~~~~~~

``handle()`` メソッドは ``Request`` オブジェクトを引数にとり、戻り値で必ず ``Response`` オブジェクトを返します。
``Request`` オブジェクトを変換するために、\ ``handle()`` メソッドは、Resolver オブジェクトと、順序付けられたイベント通知のチェーンを利用します (各イベントに関する詳細は、後の節を参照してください):

1. 何らかの処理を行う前に、\ ``kernel.request`` イベントが通知されます --
   もしリスナーの 1 つから ``Response`` オブジェクトが返された場合は、ステップ 8 まで処理をスキップします。

2. Resolver が呼び出され、実行するコントローラが決定されます。

3. ``kernel.controller`` イベントのリスナーにより、コントローラに対する任意の処理 (変更、ラッピング、その他) が行われます。

4. Kernel により、コントローラが実際にコール可能かどうかのチェックが行われます。

5. Resolver が呼び出され、コントローラへ渡す引数が決定されます。

6. カーネルからコントローラが呼び出されます。

7. コントローラから ``Response`` オブジェクトが返されない場合は、\ ``kernel.view`` イベントのリスナーにより、コントローラの戻り値の ``Response`` オブジェクトへの変換が行われます。

8. ``kernel.response`` イベントのリスナーにより、\ ``Response``\ オブジェクト (コンテンツやヘッダー) の処理が行われます。

9. クライアントへレスポンスが返されます。

処理中に例外がスローされた場合は、\ ``kernel.exception`` イベントが通知されるので、このイベントのリスナーにより例外を Response オブジェクトに変換することができます。
正常に処理が完了した場合は ``kernel.response`` イベントが通知されます。
正常に処理が完了しなかった場合は、例外が再度スローされます。

たとえば埋め込みリクエストを使っている場合に例外をキャッチされないようにするには、\ ``handle()``\ メソッドの 3 番目の引数に ``false`` を指定して、\ ``kernel.exception`` イベントを無効にしてください。

.. index::
  single: Internals; Internal Requests

内部リクエスト
~~~~~~~~~~~~~~

'マスター'\ リクエストを処理する際は、サブリクエストを処理することもできます。
``handle()`` メソッドの 2 番目の引数に、次のようなリクエストの種類を指定します:

* ``HttpKernelInterface::MASTER_REQUEST``
* ``HttpKernelInterface::SUB_REQUEST``

リクエストの種類はすべてのイベントに渡されるので、リスナーはイベントの種類に応じた処理を実行できます。
たとえば、マスターリクエストの場合にのみ処理を実行したい場合などに役に立ちます。

.. index::
   pair: Kernel; Event

イベント
~~~~~~~~

Kernel によってスローされるイベントは、\ :class:`Symfony\\Component\\HttpKernel\\Event\\KernelEvent`\ のサブクラスです。
つまり、各イベントでは次のような共通の情報にアクセスできます:

* ``getRequestType()`` - リクエストの\ *種類* (``HttpKernelInterface::MASTER_REQUEST`` または ``HttpKernelInterface::SUB_REQUEST``) を返します。

* ``getKernel()`` - リクエストを処理しているカーネルを返します。

* ``getRequest()`` - 現在処理中の ``Request`` オブジェクトを返します。

``getRequestType()``
....................

``getRequestType()`` メソッドを使って、リスナー側でリクエストの種類を取得できます。
たとえば、マスターリクエストの場合にのみリスナーの処理を実行したい場合は、リスナーメソッドの先頭に次のコードを追加してください。

::

    use Symfony\Component\HttpKernel\HttpKernelInterface;

    if (HttpKernelInterface::MASTER_REQUEST !== $event->getRequestType()) {
        // return immediately
        return;
    }

.. tip::

    Symfony2 のイベントディスパッチャーについて詳しく学びたい方は、\ :ref:`event_dispatcher` の章をお読みください。

.. index::
   single: Event; kernel.request

.. _kernel-core-request:

``kernel.request`` イベント
...........................

*イベントクラス*: :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseEvent`

このイベントは、ただちに ``Response`` オブジェクトを返すか、イベントの後に呼び出されるコントローラへ渡す変数をセットアップします。
どのリスナーも、イベントオブジェクトの ``setResponse()`` メソッドを使って ``Response`` オブジェクト返すことができます。
Response オブジェクトが返された場合、他のすべてのリスナーは呼び出されません。

このイベントは、\ ``FrameworkBundle`` で ``Request`` オブジェクトの ``_controller`` 属性の値を設定するときに、\ :class:`Symfony\\Bundle\\FrameworkBundle\\EventListener\\RouterListener` で使われます。
RequestListener では、\ :class:`Symfony\\Component\\Routing\\RouterInterface` インタフェースを実装したオブジェクトを使って ``Request`` のマッチングが行われ、コントローラ名が決定され、\ ``Request`` オブジェクトの ``_controller`` 属性に保存されます。

.. index::
   single: Event; kernel.controller

``kernel.controller`` イベント
..............................

*イベントクラス*: :class:`Symfony\\Component\\HttpKernel\\Event\\FilterControllerEvent`

このイベントは、\ ``FrameworkBundle`` では使われていません。
実行するコントローラを変更したい場合のエントリポイントとして、このイベントを使用できます。

.. code-block:: php

    use Symfony\Component\HttpKernel\Event\FilterControllerEvent;

    public function onKernelController(FilterControllerEvent $event)
    {
        $controller = $event->getController();
        // ...

        // the controller can be changed to any PHP callable
        $event->setController($controller);
    }

.. index::
   single: Event; kernel.view

``kernel.view`` イベント
........................

*イベントクラス*: :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseForControllerResultEvent`

このイベントは、\ ``FrameworkBundle`` では使われていません。
ビューサブシステムを実装したい場合に、このイベントを使用できます。
このイベントは、コントローラから ``Response`` オブジェクトが *返されなかった場合にのみ* 呼び出されます。
イベントの目的は、コントローラから任意の戻り値を返せるようにし、それを ``Response`` オブジェクトへ変換することです。

``getControllerResult`` メソッドを使うと、コントローラの戻り値にアクセスできます。

.. code-block:: php

    use Symfony\Component\HttpKernel\Event\GetResponseForControllerResultEvent;
    use Symfony\Component\HttpFoundation\Response;

    public function onKernelView(GetResponseForControllerResultEvent $event)
    {
        $val = $event->getReturnValue();
        $response = new Response();
        // some how customize the Response from the return value

        $event->setResponse($response);
    }

.. index::
   single: Event; kernel.response

``kernel.response`` イベント
............................

*イベントクラス*: :class:`Symfony\\Component\\HttpKernel\\Event\\FilterResponseEvent`

このイベントの目的は、\ ``Response`` オブジェクトが作られた後で、そのオブジェクトを他のシステムから変更または置き換えできるようにすることです。

.. code-block:: php

    public function onKernelResponse(FilterResponseEvent $event)
    {
        $response = $event->getResponse();
        // .. modify the response object
    }

``FrameworkBundle`` では、いくつかのリスナーが登録されます。

* :class:`Symfony\\Component\\HttpKernel\\EventListener\\ProfilerListener`:
  現在のリクエストのデータを収集します。

* :class:`Symfony\\Bundle\\WebProfilerBundle\\EventListener\\WebDebugToolbarListener`:
  Web デバッグツールバーを挿入します。

* :class:`Symfony\\Component\\HttpKernel\\EventListener\\ResponseListener`: Response オブジェクトの ``Content-Type`` を、リクエストのフォーマットに基づいて修正します。

* :class:`Symfony\\Component\\HttpKernel\\EventListener\\EsiListener`: Response で ESI タグのパースが必要な場合に ``Surrogate-Control`` HTTP ヘッダーを追加します。

.. index::
   single: Event; kernel.exception

.. _kernel-kernel.exception:

``kernel.exception`` イベント
.............................

*イベントクラス*: :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseForExceptionEvent`

``FrameworkBundle`` では :class:`Symfony\\Component\\HttpKernel\\EventListener\\ExceptionListener` が登録されています。
このリスナーでは、例外がスローされた場合に、指定されたコントローラへ ``Request`` をフォワードします。コントローラは ``exception_listener.controller`` パラメータに、\ ``class::method`` 形式で指定します。

このイベントのリスナーでは、\ ``Response`` オブジェクトを作成して設定するか、新しい ``Exception`` オブジェクトを作成するか、または何もしません。

.. code-block:: php

    use Symfony\Component\HttpKernel\Event\GetResponseForExceptionEvent;
    use Symfony\Component\HttpFoundation\Response;

    public function onKernelException(GetResponseForExceptionEvent $event)
    {
        $exception = $event->getException();
        $response = new Response();
        // 現在の例外に基づいて Response オブジェクトをセットアップする
        $event->setResponse($response);

        // 上記の代わりに新しい Exception オブジェクトを設定することもできる
        // $exception = new \Exception('Some special exception');
        // $event->setException($exception);
    }

.. index::
   single: Event Dispatcher

イベントディスパッチャー
------------------------

オブジェクト指向でコードを記述することは、長い間コードの拡張性を高める手法とされてきました。
責務が明確に定義されたクラスを作成することで、コードは柔軟になり、開発者がサブクラスを作成して振る舞いを変更しやすくなります。
しかし、もし開発者が自分の作成したサブクラスを、同様にサブクラスを開発した他の開発者と共有したい場合に、コードの継承には問題がでてきます。

プラグインシステムを提供する必要があるような現実のプロジェクトについて考えてみましょう。
他のプラグインの実行を阻害することなく、プラグインからメソッドを追加したり、メソッドの処理の前後に何らかの処理を実行したいでしょう。
これは単一の継承では簡単に解決できる問題ではなく、また仮に PHP で多重継承が可能であったとしても多重継承自体の欠点もあります。

Symfony2 のイベントディスパッチャーは、\ `オブザーバ`_\ パターンのシンプルで効率の良い実装です。イベントディスパッチャーにより、先に述べた問題をすべて解決でき、プロジェクトの拡張性を飛躍的に高めることができます。

`Symfony2 HttpKernel コンポーネント`_\ から簡単な例を見てみましょう。
一度 ``Response`` オブジェクトが作成された後、実際に処理される前に、たとえばキャッシュヘッダーを追加するといった Response オブジェクトの変更を、システムの他の箇所から行えると便利でしょう。
このような変更を行えるように、Symfony2 のカーネルから ``kernel.response`` イベントが通知されます。
動作は次のようになっています:

* *リスナー* (PHP オブジェクト) から\ *ディスパッチャー*\ オブジェクトへ、\ ``kernel.response`` イベントを監視することを伝えます。

* いくつかの箇所で、Symfony2 カーネルから\ *ディスパッチャー*\ オブジェクトへ、\ ``kernel.response`` イベントをディスパッチするよう伝えられます。
  この時、\ ``Response`` オブジェクトにアクセスできる ``Event`` オブジェクトがディスパッチャーに渡されます。

* ディスパッチャーにより、\ ``kernel.response`` イベントのすべてのリスナーへ通知が行われます (リスナーのメソッドの呼び出し等)。
  通知されたリスナー側では、\ ``Response`` オブジェクトを任意に変更できます。

.. index::
   single: Event Dispatcher; Events

.. _event_dispatcher:

イベント
~~~~~~~~

イベントが通知される時、イベントを監視しているリスナーの数に関わらず、イベントは \ ``kernel.response`` のようなユニークな名前で識別されます。
:class:`Symfony\\Component\\EventDispatcher\\Event` インスタンスが作成され、すべてのリスナーに渡されます。
後の節で見ていきますが、\ ``Event`` オブジェクトには、通知されるイベントに関するデータが含まれています。

.. index::
   pair: Event Dispatcher; Naming conventions

命名規約
........

ユニークなイベント名には任意の文字列を使えますが、次のような簡単な命名規約に従うことが推奨されています:

* 小文字、数字、ドット (``.``) およびアンダースコア (``_``) のみを使うようにしてください。

* プレフィックスに名前空間とドットを使います (例 ``kernel.``)。

* 実行されるアクションの内容を表す動詞で終わるようにします (例 ``request``)。

次に、良いイベント名の例を示します:

* ``kernel.response``
* ``form.pre_set_data``

.. index::
   single: Event Dispatcher; Event Subclasses

イベント名とイベントオブジェクト
................................

.. TODO 以下の翻訳をブラッシュアップ

ディスパッチャーがリスナーへ通知する時、実際の ``Event`` オブジェクトがリスナーへ渡されます。
基底の ``Event`` クラスはとても単純で、\ :ref:`イベントの伝播 <event_dispatcher-event-propagation>`\ を停止するメソッドしかありません。

通常は、特定のイベントに対するリスナーで必要とされるデータは、``Event`` オブジェクトとともにリスナーへ渡します。
``kernel.response`` イベントの場合、作成され各リスナーへ渡される ``Event`` オブジェクトは、基底の ``Event`` オブジェクトのサブクラスである :class:`Symfony\\Component\\HttpKernel\\Event\\FilterResponseEvent` のインスタンスです。
このクラスには ``getResponse`` や ``setResponse`` といったメソッドがあり、\ ``Response`` オブジェクトを取得したり置き換えたりできます。

通常のストーリーは次のようになります。
イベントに対するリスナーを作成する時、リスナーへ渡される ``Event`` オブジェクトは専用のサブクラスで、イベントから情報を取得したり、イベントに情報を返すための追加のメソッドを持っています。

ディスパッチャー
~~~~~~~~~~~~~~~~

ディスパッチャーは、イベントディスパッチャーシステムの中心となるオブジェクトです。
一般的には、ディスパッチャーオブジェクトが 1 つだけ作られ、リスナーの登録を管理します。
ディスパッチャーからイベントがディスパッチされると、そのイベントに登録されているすべてのリスナーへイベントが通知されます。

.. code-block:: php

    use Symfony\Component\EventDispatcher\EventDispatcher;

    $dispatcher = new EventDispatcher();

.. index::
   single: Event Dispatcher; Listeners

リスナーの接続
~~~~~~~~~~~~~~

既存のイベントを利用するには、リスナーをディスパッチャーへ接続しておきます。
こうすると、対象のイベントがディスパッチされた時に通知されるようになります。
ディスパッチャーの ``addListener()`` メソッドを呼び出して、関数としてコール可能な任意の PHP コードをイベントに関連付けることができます。

.. code-block:: php

    $listener = new AcmeListener();
    $dispatcher->addListener('foo.action', array($listener, 'onFooAction'));

``addListener()`` メソッドは、次の 3 つの引数をとります。

* イベント名 (文字列) リスナーの監視対象とするイベント。

* イベントが通知された場合に呼び出される PHP コード。

* 任意で、優先度を示す整数値 (大きい方がより重要) この値により、リスナーが複数登録されている場合に通知される順番が決まります (デフォルトは ``0``)。
  2 つのリスナーが同じ優先度を指定した場合、ディスパッチャーに追加された順に実行されます。

.. note::

    `関数としてコール可能な PHP コード`_\ とは、PHP の変数で、\ ``call_user_func()`` 関数のパラメータに使用でき、\ ``is_callable()`` 関数に渡した場合は ``true`` が返されます。
    これには、\ ``\Closure`` のインスタンス、関数を指定する文字列、オブジェクトのメソッドやクラスのメソッドを表す配列表現を使えます。

    リスナーとして PHP オブジェクトを登録する方法について説明してきましたが、同じように、イベントリスナーとして `Closures`_ を登録することもできます。

    .. code-block:: php

        use Symfony\Component\EventDispatcher\Event;

        $dispatcher->addListener('foo.action', function (Event $event) {
            // foo.action イベントがディスパッチされた時に実行される
        });

リスナーはディスパッチャーへ登録された後、イベントが通知されるまで待機します。
上の例では、\ ``foo.action`` イベントがディスパッチされると、ディスパッチャーから ``AcmeListener::onFooAction`` メソッドが呼び出され、単一の引数として ``Event`` オブジェクトが渡されます。

.. code-block:: php

    use Symfony\Component\EventDispatcher\Event;

    class AcmeListener
    {
        // ...

        public function onFooAction(Event $event)
        {
            // do something
        }
    }

.. tip::

    Symfony2 MVC フレームワークを使っている場合は、\ :ref:`設定ファイル <dic-tags-kernel-event-listener>` を使ってリスナーを登録できます。
    また、リスナーオブジェクトは必要な場合にのみインスタンス化されます。

多くの場合、リスナーには指定されたイベント独自の ``Event`` のサブクラスが渡されます。
これにより、イベントに関する特別な情報にリスナーからアクセスできます。
リスナーに渡されているイベントが、実際にどの ``Symfony\Component\EventDispatcher\Event`` インスタンスなのかを判断するには、ドキュメントかソースコードを確認してください。
たとえば、\ ``kernel.event`` イベントは ``Symfony\Component\HttpKernel\Event\FilterResponseEvent`` のインスタンスを渡します。

.. code-block:: php

    use Symfony\Component\HttpKernel\Event\FilterResponseEvent

    public function onKernelResponse(FilterResponseEvent $event)
    {
        $response = $event->getResponse();
        $request = $event->getRequest();

        // ...
    }

.. _event_dispatcher-closures-as-listeners:

.. index::
   single: Event Dispatcher; Creating and Dispatching an Event

イベントの作成とディスパッチ
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

既存のイベントに対してリスナーを登録するだけでなく、独自のイベントを作成して通知することもできます。サードパーティライブラリを作ったり、システム内のさまざまなコンポーネントを柔軟で疎結合に保ちたい場合に役に立ちます。

静的な ``Events`` クラス
........................

ここでは仮に、新しいイベント - ``store.order`` - を作成してみましょう。
このイベントは、アプリケーションで注文が行われるたびにディスパッチされます。
うまくまとめるためには、アプリケーション内に ``StoreEvents`` クラスを作成し、ここにイベントの定義やドキュメントを記述していきましょう。

.. code-block:: php

    namespace Acme\StoreBundle;

    final class StoreEvents
    {
        /**
         * システムで注文が行われるたびに store.order イベントが通知される。
         *
         * イベントリスナーは Acme\StoreBundle\Event\FilterOrderEvent のインスタンスを受け取る。
         *
         * @var string
         */
        const onStoreOrder = 'store.order';
    }

このクラスは、実際には何も処理を行わないことに注意してください。
``StoreEvents`` クラスの目的は、共通イベントに関する情報を集中管理することです。
このイベントに対する各リスナーには、特別な ``FilterOrderEvent`` クラスのオブジェクトも渡されます。

イベントオブジェクトの作成
..........................

次に、この新しいイベントをディスパッチする時、\ ``Event`` インスタンスを作成してディスパッチャーへ渡します。
ディスパッチャーは渡された Event インスタンスを、イベントの各リスナーへ渡します。
リスナーに何も情報を渡す必要がない場合は、デフォルトの ``Symfony\Component\EventDispatcher\Event`` クラスをそのまま使えます。
しかし、イベントに関する何らかの情報をリスナーへ渡したい場合が多いでしょう。
このような場合は、\ ``Symfony\Component\EventDispatcher\Event`` を継承する新しいクラスを作成します。

この例では、各リスナーは架空の ``Order`` オブジェクトへアクセスする必要があるとします。
次のように ``Event`` クラスを作成します。

.. code-block:: php

    namespace Acme\StoreBundle\Event;

    use Symfony\Component\EventDispatcher\Event;
    use Acme\StoreBundle\Order;

    class FilterOrderEvent extends Event
    {
        protected $order;

        public function __construct(Order $order)
        {
            $this->order = $order;
        }

        public function getOrder()
        {
            return $this->order;
        }
    }

各リスナーは、イベントオブジェクトの ``getOrder`` メソッドを使って ``Order`` オブジェクトにアクセスできます。

イベントのディスパッチ
......................

:method:`Symfony\\Component\\EventDispatcher\\EventDispatcher::dispatch` メソッドを呼び出すと、指定されたイベントのすべてのリスナーに対して通知します。
このメソッドは、次の 2 つの引数をとります。ディスパッチするイベントの名前と、イベントのリスナーに渡される ``Event`` インスタンスです。

.. code-block:: php

    use Acme\StoreBundle\StoreEvents;
    use Acme\StoreBundle\Order;
    use Acme\StoreBundle\Event\FilterOrderEvent;

    // 注文を何らかの方法で作成または取得する
    $order = new Order();
    // ...

    // FilterOrderEvent を作成し、ディスパッチする
    $event = new FilterOrderEvent($order);
    $dispatcher->dispatch(StoreEvents::onStoreOrder, $event);

このコードでは、特別な ``FilterOrderEvent`` オブジェクトを作成し、\ ``dispatch`` メソッドへ渡しています。
これで、\ ``store.order`` イベントに対するすべてのリスナーが ``FilterOrderEvent`` を受け取り、\ ``getOrder`` メソッドを使って ``Order`` オブジェクトへアクセスできます。

.. code-block:: php

    // onStoreOrderイベント用に登録するリスナークラス
    use Acme\StoreBundle\Event\FilterOrderEvent;

    public function onStoreOrder(FilterOrderEvent $event)
    {
        $order = $event->getOrder();
        // 注文に対する何らかの処理
    }

イベントディスパッチャーオブジェクトを伝える
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

``EventDispatcher`` クラスのソースコードを読むと、シングルトンではないことに気づくでしょう (``getInstance()`` スタティックメソッドは定義されていません)。
これは設計者の意図で、単一の PHP リクエストの処理中に複数のイベントディスパッチャーを同時に実行したい場合に役立ちます。
その代わりに、イベントへの接続や通知を行う必要があるオブジェクトへ、ディスパッチャーオブジェクトを渡す必要があります。

この問題に対するベストプラクティスは、対象となるオブジェクトへディスパッチャーオブジェクトを注入することで、これを依存性の注入 (dependency injection) と呼びます。

コンストラクタでの注入は、次のようになります。

.. code-block:: php

    class Foo
    {
        protected $dispatcher = null;

        public function __construct(EventDispatcher $dispatcher)
        {
            $this->dispatcher = $dispatcher;
        }
    }

セッターで注入する方法もあります。

.. code-block:: php

    class Foo
    {
        protected $dispatcher = null;

        public function setEventDispatcher(EventDispatcher $dispatcher)
        {
            $this->dispatcher = $dispatcher;
        }
    }

どちらの方法を選ぶかは、好みの問題です。
オブジェクトの生成時に初期化を完了できるようになるので、多くの人はコンストラクタによる注入を利用します。
しかし、依存が多くリストが長くなってしまう場合や、任意の依存があるような場合は、セッターによる注入を使うとよいでしょう。

.. tip::

    上で述べた 2 つの例のような依存性注入を利用する場合は、\ `Symfony2 Dependency Injection コンポーネント`_\ を使うと、とてもきれいにオブジェクトを管理できます。

.. index::
   single: Event Dispatcher; Event subscribers

イベントサブスクライバを使う
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

イベントを監視する最も一般的な方法は、\ *イベントリスナー*\ をディスパッチャーに登録することです。
リスナーは 1 つ以上のイベントを監視でき、イベントがディスパッチされるたびに通知されます。

イベントを監視するもう 1 つの方法が、\ *イベントサブスクライバ* です。
イベントサブスクライバは PHP のクラスで、ディスパッチャーに対してどのイベントが監視されるのかを、サブスクライバクラス自身が通知します。
サブスクライバクラスは :class:`Symfony\\Component\\EventDispatcher\\EventSubscriberInterface` インタフェース、および ``getSubscribedEvents`` というスタティックメソッドを実装します。
次に示すサブスクライバクラスの例では、\ ``kernel.response`` イベントと ``store.order`` イベントを監視します。

.. code-block:: php

    namespace Acme\StoreBundle\Event;

    use Symfony\Component\EventDispatcher\EventSubscriberInterface;
    use Symfony\Component\HttpKernel\Event\FilterResponseEvent;

    class StoreSubscriber implements EventSubscriberInterface
    {
        static public function getSubscribedEvents()
        {
            return array(
                'kernel.response' => 'onKernelResponse',
                'store.order'     => 'onStoreOrder',
            );
        }

        public function onKernelResponse(FilterResponseEvent $event)
        {
            // ...
        }

        public function onStoreOrder(FilterOrderEvent $event)
        {
            // ...
        }
    }

これはリスナークラスとよく似ていますが、監視するイベントの種類をクラス自身がディスパッチャーに対して通知できる点が異なります。
ディスパッチャーにサブスクライバを登録するには、\ :method:`Symfony\\Component\\EventDispatcher\\EventDispatcher::addSubscriber` メソッドを使います。

.. code-block:: php

    use Acme\StoreBundle\Event\StoreSubscriber;

    $subscriber = new StoreSubscriber();
    $dispatcher->addSubscriber($subscriber);

ディスパッチャーにより、サブスクライバクラスの ``getSubscribedEvents`` メソッドで返された各イベントに対して、自動的に登録が行われます。
リスナーと同様に、\ ``addSubscriber`` メソッドには任意で指定できる 2 つ目の引数があり、イベントへ渡す優先度を指定できます。

.. index::
   single: Event Dispatcher; Stopping event flow

.. _event_dispatcher-event-propagation:

イベントの流れ/伝播を停める
~~~~~~~~~~~~~~~~~~~~~~~~~~~

リスナーの処理後に、他のリスナーが呼び出されないようにしたい場合もあります。
つまり、リスナーからディスパッチャーに対して、以降のリスナーへのイベントの伝播を停止する (これ以上リスナーへ通知を行わない) よう要求できるようにしたいのです。
このような場合は、リスナーから :method:`Symfony\\Component\\EventDispatcher\\Event::stopPropagation` method: メソッドを呼び出します。

.. code-block:: php

   use Acme\StoreBundle\Event\FilterOrderEvent;

   public function onStoreOrder(FilterOrderEvent $event)
   {
       // ...

       $event->stopPropagation();
   }

こうすると、\ ``store.order`` イベントに対して登録されているリスナーのうちまだ呼び出されていないリスナーがあっても、呼び出されなくなります。

.. index::
   single: Profiler

プロファイラ
------------

Symfony2 のプロファイラを有効にすると、アプリケーションに対してリクエストがあるたびに、リクエストに関する有益な情報を収集し、後で解析できるように保存します。
開発環境でプロファイラを使うことで、コードのデバッグやパフォーマンス改善が容易になります。
運用環境でプロファイラを使うと、問題が発生した場合の状況の把握などに役立ちます。

Symfony2 には Web デバッグツールバーや Web プロファイラといったプロファイル情報の視覚化ツールがあるので、開発者がプロファイラを直接操作する必要はほとんどありません。
Symfony2 Standard Edition を使っている場合、プロファイラ、Web デバッグツールバー、Web プロファイラは、最初から使えるように設定されています。

.. note::

    プロファイラは、単純なリクエスト、リダイレクト、例外、Ajax リクエスト、ESI リクエスト、およびすべての HTTP メソッドとフォーマットといった、リクエストに対するあらゆる情報を収集します。
    リクエストとレスポンスの組み合わせ 1 つについて 1 つのプロファイリングデータが生成されるため、1 つの URL に対して複数のプロファイリングデータが生成される場合もあります。

.. index::
   single: Profiler; Visualizing

プロファイリングデータの視覚化
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Web デバッグツールバーを使う
............................

開発環境では、すべてのページの下部に Web デバッグツールバーが表示されます。
Web デバッグツールバーにはプロファイリングデータの要約が表示されており、アプリケーションの動作が意図したものと異なる場合に、原因を手軽に調べることができます。

Web デバッグツールバーに表示される要約では情報が不足している場合は、ツールバーのトークン文字列 (13 文字のランダムな文字列) をクリックして Web プロファイラへアクセスしてください。

.. note::

    トークン文字列にリンクが設定されていない場合は、プロファイラ用のルートが登録されていません。
    次の節でプロファイラの設定を確認してください。

Web プロファイラを使ったプロファイルデータの解析
................................................

Web プロファイラは、開発環境でコードのデバッグとパフォーマンスの改善に使えるプロファイリングデータの視覚化ツールです。
運用環境で問題を発見する目的で使うこともできます。
Web プロファイラを使うと、プロファイラが収集したすべての情報を Web インタフェースで閲覧することができます。

.. index::
   single: Profiler; Using the profiler service

プロファイル情報へのアクセス
............................

Symfony2 のプロファイリングデータは、デフォルトの視覚化ツール (Web プロファイラ) 以外のツールからもアクセスできます。
では、どうやって特定のリクエストに関するプロファイリングデータを抽出すればよいのでしょうか。
プロファイラは、リクエストのプロファイルデータを保存する際、そのデータに関連付けられたトークンも保存します。
このトークンは、レスポンス内の HTTP ヘッダにある ``X-Debug-Token`` から取得できます。

.. code-block:: php

    $profile = $container->get('profiler')->loadProfileFromResponse($response);

    $profile = $container->get('profiler')->loadProfile($token);

.. tip::

    プロファイラを有効にしている環境で、Web デバッグツールバーを無効にしていたり、Ajax リクエストのトークンを取得したい場合は、
    Firebug などのツールを使って ``X-Debug-Token`` HTTP ヘッダの値を調べてください。

``find()`` メソッドを使うと、何らかの条件にマッチするアクセストークンの一覧を取得できます。

.. code-block:: php

    // 最新の 10 個のトークンを取得する
    $tokens = $container->get('profiler')->find('', '', 10);

    // URL に /admin/ を含む最新の 10 個のトークンを取得する
    $tokens = $container->get('profiler')->find('', '/admin/', 10);

    // ローカルリクエストから最新の 10 個のトークンを取得する
    $tokens = $container->get('profiler')->find('127.0.0.1', '', 10);

プロファイリングデータが生成されたサーバー環境とは異なるサーバー上でデータの解析を行いたい場合は、\ ``export()`` メソッドと ``import()`` メソッドを使います。

.. code-block:: php

    // 運用環境でエクスポートする
    $profile = $container->get('profiler')->loadProfile($token);
    $data = $profiler->export($profile);

    // 開発環境でインポートする
    $profiler->import($data);

.. index::
   single: Profiler; Visualizing

設定
....

Symfony2 にはデフォルトで、プロファイラ、Web デバッグツールバー、Web プロファイラの実用的な設定が組み込まれています。
開発環境向けの設定は、次のようになっています。

.. configuration-block::

    .. code-block:: yaml

        # load the profiler
        framework:
            profiler: { only_exceptions: false }

        # enable the web profiler
        web_profiler:
            toolbar: true
            intercept_redirects: true
            verbose: true

    .. code-block:: xml

        <!-- xmlns:webprofiler="http://symfony.com/schema/dic/webprofiler" -->
        <!-- xsi:schemaLocation="http://symfony.com/schema/dic/webprofiler http://symfony.com/schema/dic/webprofiler/webprofiler-1.0.xsd"> -->

        <!-- load the profiler -->
        <framework:config>
            <framework:profiler only-exceptions="false" />
        </framework:config>

        <!-- enable the web profiler -->
        <webprofiler:config
            toolbar="true"
            intercept-redirects="true"
            verbose="true"
        />

    .. code-block:: php

        // load the profiler
        $container->loadFromExtension('framework', array(
            'profiler' => array('only-exceptions' => false),
        ));

        // enable the web profiler
        $container->loadFromExtension('web_profiler', array(
            'toolbar' => true,
            'intercept-redirects' => true,
            'verbose' => true,
        ));

``only-exceptions`` を ``true`` に設定すると、アプリケーションから例外がスローされた場合にのみプロファイリングデータが収集されます。

``intercept-redirects`` を ``true`` に設定すると、Web プロファイラによってアプリケーションのリダイレクトが捕捉されるようになります。
これにより、アプリケーションがリダイレクト先へ遷移する前に、収集されたプロファイリングデータを確認できます。

``verbose`` を ``true`` に設定すると、Web デバッグツールバーにより多くの情報が表示されるようになります。
``verbose`` を ``false`` に設定すると、2 次的な情報は非表示になります。

Web プロファイラを有効にする場合は、プロファイラ用のルートも同時に有効にしておく必要があります。

.. configuration-block::

    .. code-block:: yaml

        _profiler:
            resource: @WebProfilerBundle/Resources/config/routing/profiler.xml
            prefix:   /_profiler

    .. code-block:: xml

        <import resource="@WebProfilerBundle/Resources/config/routing/profiler.xml" prefix="/_profiler" />

    .. code-block:: php

        $collection->addCollection($loader->import("@WebProfilerBundle/Resources/config/routing/profiler.xml"), '/_profiler');

プロファイラを有効にすると、わずかながｒオーバーヘッドが発生するので、特に運用環境では特定の条件下でのみ有効となるようにしたいでしょう。
``only-exceptions`` 設定で、プロファイリングするページを 500 ページに制限できます。
特定の IP アドレスからのアクセスのみプロファイリングしたり、Web サイト内の一部分に対してのみプロファイリングしたい場合は、リクエストマッチャーを使います。

.. configuration-block::

    .. code-block:: yaml

        # 192.168.0.0 ネットワーク内からのリクエストでのみプロファイラを有効にする
        framework:
            profiler:
                matcher: { ip: 192.168.0.0/24 }

        # /admin 以下の URL に対してのみプロファイラを有効にする
        framework:
            profiler:
                matcher: { path: "^/admin/" }

        # 複数のルールを結合する
        framework:
            profiler:
                matcher: { ip: 192.168.0.0/24, path: "^/admin/" }

        # "custom_matcher" サービスで定義されたカスタムマッチャーインスタンスを使う
        framework:
            profiler:
                matcher: { service: custom_matcher }

    .. code-block:: xml

        <!-- 192.168.0.0 ネットワーク内からのリクエストでのみプロファイラを有効にする -->
        <framework:config>
            <framework:profiler>
                <framework:matcher ip="192.168.0.0/24" />
            </framework:profiler>
        </framework:config>

        <!-- /admin 以下の URL に対してのみプロファイラを有効にする -->
        <framework:config>
            <framework:profiler>
                <framework:matcher path="^/admin/" />
            </framework:profiler>
        </framework:config>

        <!-- 複数のルールを結合する -->
        <framework:config>
            <framework:profiler>
                <framework:matcher ip="192.168.0.0/24" path="^/admin/" />
            </framework:profiler>
        </framework:config>

        <!-- "custom_matcher" サービスで定義されたカスタムマッチャーインスタンスを使う -->
        <framework:config>
            <framework:profiler>
                <framework:matcher service="custom_matcher" />
            </framework:profiler>
        </framework:config>

    .. code-block:: php

        // 192.168.0.0 ネットワーク内からのリクエストでのみプロファイラを有効にする
        $container->loadFromExtension('framework', array(
            'profiler' => array(
                'matcher' => array('ip' => '192.168.0.0/24'),
            ),
        ));

        // /admin 以下の URL に対してのみプロファイラを有効にする
        $container->loadFromExtension('framework', array(
            'profiler' => array(
                'matcher' => array('path' => '^/admin/'),
            ),
        ));

        // 複数のルールを結合する
        $container->loadFromExtension('framework', array(
            'profiler' => array(
                'matcher' => array('ip' => '192.168.0.0/24', 'path' => '^/admin/'),
            ),
        ));

        # "custom_matcher" サービスで定義されたカスタムマッチャーインスタンスを使う
        $container->loadFromExtension('framework', array(
            'profiler' => array(
                'matcher' => array('service' => 'custom_matcher'),
            ),
        ));

クックブックでさらに学ぶ
------------------------

* :doc:`/cookbook/testing/profiling`
* :doc:`/cookbook/profiler/data_collector`
* :doc:`/cookbook/event_dispatcher/class_extension`
* :doc:`/cookbook/event_dispatcher/method_behavior`

.. _オブザーバ: http://en.wikipedia.org/wiki/Observer_pattern
.. _`Symfony2 HttpKernel コンポーネント`: https://github.com/symfony/HttpKernel
.. _Closures: http://php.net/manual/en/functions.anonymous.php
.. _`Symfony2 Dependency Injection コンポーネント`: https://github.com/symfony/DependencyInjection
.. _`関数としてコール可能な PHP コード`: http://www.php.net/manual/en/language.pseudo-types.php#language.types.callback
