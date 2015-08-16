.. index::
   single: Controller

.. note::

    * 対象バージョン：2.4 (2.2以降)
    * 翻訳更新日：2014/1/1

コントローラー
==============

コントローラーは、HTTP リクエストから情報を取得し、\
(Symfony2 の ``Response`` オブジェクトとして) HTTP レスポンスを作成して返す PHP 関数です。\
レスポンスは、HTML ページ、XML ドキュメント、シリアライズされた JSON 配列、\
画像、リダイレクト、404 エラー、その他思いつく物なんでもよいでしょう。\
コントローラーには、\ *あなたのアプリケーション*\ が、ページコンテンツをレンダリングする際に必要な任意のロジックが盛り込まれます。

このことがどれだけシンプルなことなのか、\
Symfony2 コントローラーが実際に動作しているところを見ていきましょう。\
次のコードは、\ ``Hello world!`` と出力するページをレンダリングするコントローラーの例です。

.. code-block:: php

    use Symfony\Component\HttpFoundation\Response;

    public function helloAction()
    {
        return new Response('Hello world!');
    }

コントローラーの表面的な役割は常に 1 つです。\
すなわち、\ ``Response`` オブジェクトを作成して返すことです。\
そこに至るまでに、リクエストからの情報の読み込みや、データベースリソースの読み込み、\
メールの送信、ユーザーのセッションへの情報の設定を行うかもしれませんが、\
どの場合においても、コントローラーは最終的にはクライアントに届けることになる ``Response`` オブジェクトを返します。

いくつかよくある例を見てみましょう。

* *コントローラー A* サイトのトップページコンテンツである ``Response`` オブジェクトを準備する。

* *コントローラー B* リクエストから ``slug`` パラメーターを読み込み、\
  データベースからブログエントリを取得するためにそれを使用し、\
  そのブログを表示する ``Response`` オブジェクトを作成する。\
  もし ``slug`` がデータベース中に見つからなければ、ステータスコード 404 の ``Response`` オブジェクトを作成します。

* *コントローラー C* お問い合わせフォームの送信を扱う。\
  リクエストからフォームの情報を読み込み、データベースにセーブし、\
  ウェブマスターにその情報をメールします。\
  最終的には、クライアントのブラウザをお礼ページにリダイレクトさせる ``Response`` オブジェクトを作成します。

.. index::
   single: Controller; Request-controller-response lifecycle

リクエスト、コントローラー、レスポンスのライフサイクル
----------------------------------------------------

Symfony2 プロジェクトによって処理されるリクエストは、全て同じシンプルなライフサイクルに従っています。\
フレームワークが、なんども繰り返されるようなタスクを解決し、\
あなたの作成したコードが格納されているコントローラーを実行します。

#. 各リクエストは、単一のフロントコントローラーファイル(``app.php`` や ``app_dev.php``)によって処理され、アプリケーションがブートされます。

#. ``Router`` が、リクエストから情報(URI)を読み込み、その情報にマッチするルートを見つけ、そのルートの ``_controller`` パラメーターを読み込みます。

#. マッチしたルートのコントローラーが実行され、コントローラー内のコードにより ``Response`` オブジェクトが作成され、返されます。

#. HTTP ヘッダと ``Response`` オブジェクトのコンテンツがクライアントに送り返されます。

ページを作成するということはとても簡単で、コントローラーの作成(#3)を行い、URL をそのコントローラーにマップするルートの作成(#2)を行うということになります。

.. note::

    似たような名前ですが、「フロントコントローラー」は、この章で話す「コントローラー」とは別ものです。\
    フロントコントローラーは、小さな PHP ファイルで公開領域に配置されており、全てのリクエストが通過します。\
    典型的なアプリケーションでは、プロダクション用のフロントコントローラー(``app.php``)と、\
    開発用のフロントコントローラー(``app_dev.php``)が存在するでしょう。\
    このファイルを編集したり参照したり思い悩むことはおそらく無いでしょう。

.. index::
   single: Controller; Simple example

シンプルなコントローラー
------------------------

コントローラーは、PHP の Callable (関数、オブジェクトのメソッド、\ ``クロージャー``) であればよく、\
特に Symfony2 では、コントローラークラス内の 1 つのメソッドのことを指します。\
コントローラーは、\ *アクション*\ とも呼びます。

.. code-block:: php
    :linenos:

    // src/Acme/HelloBundle/Controller/HelloController.php
    namespace Acme\HelloBundle\Controller;

    use Symfony\Component\HttpFoundation\Response;

    class HelloController
    {
        public function indexAction($name)
        {
            return new Response('<html><body>Hello '.$name.'!</body></html>');
        }
    }

.. tip::

    *コントローラー*\ は ``indexAction`` メソッドであり、\ *コントローラークラス*\ (``HelloController``) 内に存在していることに注意してください。\
    *コントローラークラス*\ と呼ぶのは、単に複数のコントローラー/アクションをグループ化するのに便利だからです。\
    典型的にはコントローラークラスは複数のコントローラー/アクションを内包することになるでしょう(``updateAction`` や ``deleteAction`` など)。

このコントローラーはとても単純です。

* *4 行目*: Symfony2 は PHP 5.3 の名前空間機能をうまく利用して、コントローラークラス全体を名前空間付けしています。\
  ``use`` キーワードで、コントローラーの戻り値として利用する ``Response`` クラスをインポートしています。

* *6 行目*: クラス名は、そのコントローラーの名前 (``Hello``) と ``Controller`` という文字列を結合したものです。\
  これは、コントローラー群に一貫性を提供し、ルーティング設定の際に最初の部分 (``Hello``) のみの参照で済むようにするための慣習です。

* *8 行目*: コントローラークラス内の各アクションは、サフィックスとして ``Action`` を付けます。\
  ルーティングの設定では、アクション名 (``index``) のみで参照されます。\
  次節では、ルート (URI をアクションにマッピングする) を作成しますが、\
  そのルートのプレースホルダ (``{name}``) が、どうやってアクションメソッドの引数 (``$name``) になっていくのかを見ていきます。

* *10 行目*: コントローラーが ``Response`` オブジェクトを作成して返します。

.. index::
   single: Controller; Routes and controllers

URL をコントローラーにマッピングする
------------------------------------

先程のコントローラーはシンプルな HTML ページを返します。\
ですが、実際にブラウザで確認するには、ルートを作成する必要があります。\
ルートは、特定の URL パスをコントローラーにマッピングするものです。

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        hello:
            path:      /hello/{name}
            defaults:  { _controller: AcmeHelloBundle:Hello:index }

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="hello" path="/hello/{name}">
                <default key="_controller">AcmeHelloBundle:Hello:index</default>
            </route>
        </routes>

    .. code-block:: php

        // app/config/routing.php
        $collection->add('hello', new Route('/hello/{name}', array(
            '_controller' => 'AcmeHelloBundle:Hello:index',
        )));

.. note::

   ルート定義に使う ``path`` オプションは、Symfony 2.2 以降のものです。
   Symfony 2.1 までは ``pattern`` というオプションでした。

これで、\ ``/hello/ryan`` にアクセスすると ``HelloController::indexAction()`` コントローラーが実行され、\
``$name`` 変数として ``ryan`` が渡されるようになります。\
Symfony において「ページ」を作成するということは、単純にコントローラーメソッドを作成し、ルートと関連付けることを意味します。

コントローラーを指定する構文 ``AcmeHelloBundle:Hello:index`` に注意してください。\
Symfony2 はコントローラーを指定するために、柔軟な記法を使用しています。\
この例は最も一般的な構文で、``AcmeHelloBundle`` という名前のバンドルにある ``HelloController`` というクラスを探すように伝えています。\
そして、\ ``indexAction()`` メソッドが実行されます。

コントローラーを指定する文字列フォーマットについて、詳細は :ref:`controller-string-syntax` を参照してください。

.. note::

    この例では、\ ``app/config/`` ディレクトリにある ``routing.yml`` にルート設定を直接記述していますが、\
    ルートが属しているバンドル配下のファイルに記述することをおすすめします。\
    詳細は :ref:`routing-include-external-resources` を参照してください。

.. tip::

    ルーティングに関してもっと学びたい場合は、\ :doc:`ルーティング </book/routing>`\ を参照してください。

.. index::
   single: Controller; Controller arguments

.. _route-parameters-controller-arguments:

ルートパラメーターとコントローラーの引数の関係
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

``_controller`` パラメーターである ``AcmeHelloBundle:Hello:index`` が、\ ``AcmeHelloBundle`` 内の ``HelloController::indexAction()`` メソッドを指定していることはわかりましたね。\
もっとおもしろいのは、そのメソッドに渡される引数の話です。

.. code-block:: php

    // src/Acme/HelloBundle/Controller/HelloController.php
    namespace Acme\HelloBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class HelloController extends Controller
    {
        public function indexAction($name)
        {
          // ...
        }
    }

このコントローラーは ``$name`` 引数をもっており、マッチしたルートの ``{name}`` パラメーターに対応しています (今回の例では ``ryan`` という値)。\
実際には、コントローラーが実行されるとき、Symfony2 はコントローラーの各引数とルートのパラメーターをマッチさせています。\
次の例を見てください。

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        hello:
            path:      /hello/{firstName}/{lastName}
            defaults:  { _controller: AcmeHelloBundle:Hello:index, color: green }

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="hello" path="/hello/{firstName}/{lastName}">
                <default key="_controller">AcmeHelloBundle:Hello:index</default>
                <default key="color">green</default>
            </route>
        </routes>

    .. code-block:: php

        // app/config/routing.php
        $collection->add('hello', new Route('/hello/{firstName}/{lastName}', array(
            '_controller' => 'AcmeHelloBundle:Hello:index',
            'color'       => 'green',
        )));

この例のルート用のコントローラーは、複数の引数をとることができます。

.. code-block:: php

    public function indexAction($firstName, $lastName, $color)
    {
        // ...
    }

プレースホルダー変数 (``{firstName}``, ``{lastName}``) もそうですが、\
デフォルトの ``color`` 変数も、コントローラーの引数として有効です。\
ルートがマッチした際に、プレースホルダーの変数が、\ ``defaults`` と共にマージされ、\
コントローラー内で使用できるように 1 つの配列として作成されます。\

ルートパラメーターをコントローラーの引数にマッピングするのはとても簡単で柔軟性があります。\
開発時に下記のガイドラインを心に留めておいてください。

* **コントローラーの引数の順番は関係ない**

    Symfony は、ルートのパラメーター名を、コントローラーメソッドの表記にある変数名にマッチさせることができます。\
    言い換えると、\ ``{lastName}`` パラメーターは ``$lastName`` にマッチするということです。\
    コントローラーの引数の順は、全く異なる順番でもうまく動きます。

    .. code-block:: php

       public function indexAction($lastName, $color, $first_name)
       {
           // ...
       }

* **必須な引数は必ずルーティングパラメーターとマッチする必要がある**

    次の例では ``RuntimeException`` がスローされます。\
    ルートに ``foo`` パラメーターが定義されていないからです。

    .. code-block:: php

       public function indexAction($firstName, $lastName, $color, $foo)
       {
           // ...
       }

    ただし、オプションにしてしまえば全く問題ありません。\
    次の例では例外はスローされません。

    .. code-block:: php

       public function indexAction($firstName, $lastName, $color, $foo = 'bar')
       {
           // ...
       }

* **全てのルーティングパラメーターをコントローラー引数に使う必要はない**

    例えば、\ ``last_name`` パラメーターがコントローラーにとって重要でないのであれば、完全に省略しても大丈夫です。

    .. code-block:: php

        public function indexAction($first_name, $color)
        {
            // ...
        }

.. tip::

    全てのルートは特別なパラメーターである ``_route`` を持っています。\
    このパラメーターは、マッチしたルートの名前(``hello``)を意味します。\
    いつも便利かというとそうでもありませんが、これもコントローラーの引数として同様に有効です。

.. _book-controller-request-argument:

コントローラー引数としての ``Request``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

簡単にするため、コントローラーに引数として ``Request`` オブジェクトを渡すようにすることも可能です。\
特にフォームを扱っている場合に便利です。次の例を見てください。

.. code-block:: php

    use Symfony\Component\HttpFoundation\Request;

    public function updateAction(Request $request)
    {
        $form = $this->createForm(...);

        $form->handleRequest($request);
        // ...
    }

.. index::
   single: Controller; Base controller class

静的ページの作成
----------------

コントローラーを利用せず、ルートとテンプレートのみで静的ページを作ることもできます。

:doc:`/cookbook/templating/render_without_controller` を参照してください。

基底コントローラークラス
------------------------

Symfony2 には、基底 ``Controller`` クラスが用意されています。\
このクラスは、一般的なコントローラータスクを補助してくれたり、\
必要となるであろうあらゆるリソースへのアクセスを提供してくれます。\
この ``Controller`` クラスを継承することで、いくつかのヘルパメソッドを有効利用することができます。
``Controller`` クラスの先頭に ``use`` ステイトメントを付加し、\
それを継承するように ``HelloController`` を変更します。

.. code-block:: php

    // src/Acme/HelloBundle/Controller/HelloController.php
    namespace Acme\HelloBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use Symfony\Component\HttpFoundation\Response;

    class HelloController extends Controller
    {
        public function indexAction($name)
        {
            return new Response('<html><body>Hello '.$name.'!</body></html>');
        }
    }

この時点では特にコントローラーの処理が変わったわけではありません。\
次節では、基底コントローラークラスの存在により使用可能となるヘルパーメソッドを見ていきます。\
これらのメソッドは、単に Symfony2 の機能へのショートカットです。\
その機能自体は、基底 ``Controller`` クラスを通しても通さなくても利用可能です。\
実際のコア機能を参照するには、\ :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller` を見るとよいでしょう。

.. tip::

    Symfony では、この基底クラスを継承するのは *オプション*\ です。\
    便利なショートカットはありますが、強制ではありません。\
    :class:`Symfony\\Component\\DependencyInjection\\ContainerAware` クラスを継承したり、
    PHP 5.4 以降であれば :class:`Symfony\\Component\\DependencyInjection\\ContainerAwareTrait` トレイトを使うこともできます。
    こうすると ``container`` プロパティ経由でサービスコンテナにアクセスできます。

.. versionadded:: 2.4
    ``ContainerAwareTrait`` は Symfony 2.4 で追加されました。

.. note::

    :doc:`コントローラーをサービスとして定義する </cookbook/controller/service>`\ こともできます。
    コントローラーで利用する依存を明示的に制御できるようになります。

.. index::
   single: Controller; Common tasks

一般的なコントローラータスク
----------------------------

コントローラーが実質的になんでもできるとは言っても、ほとんどのコントローラーでは、\
同じ基礎的なタスクを何度も何度も行うことになるでしょう。\
Symfony2 では、リダイレクトやフォワード、テンプレートのレンダリング、コアサービスへのアクセスといったことを、\
とても簡単に扱うことができます。

.. index::
   single: Controller; Redirecting

リダイレクト
~~~~~~~~~~~~

ユーザーを別のページへリダイレクトさせたいときは、\ ``redirect()`` メソッドを使用します。

.. code-block:: php

    public function indexAction()
    {
        return $this->redirect($this->generateUrl('homepage'));
    }

``generateUrl()`` メソッドは、与えられたルートに対する URL を生成するヘルパ関数です。\
詳細な情報は、\ :doc:`ルーティング </book/routing>` 章を参照してください。

デフォルトでは、\ ``redirect()`` メソッドは 302(temporary) リダイレクトとして動作します。\
301(permanent) としてリダイレクトさせるには、第二引数を変更します。

.. code-block:: php

    public function indexAction()
    {
        return $this->redirect($this->generateUrl('homepage'), 301);
    }

.. tip::

    ``redirect()`` メソッドは、ユーザーをリダイレクトさせることに特化した ``Resoponse`` オブジェクトを作成するショートカットです。\
    これは、次のコードと同等です。

    .. code-block:: php

        use Symfony\Component\HttpFoundation\RedirectResponse;

        return new RedirectResponse($this->generateUrl('homepage'));

.. index::
   single: Controller; Forwarding

フォワード
~~~~~~~~~~

内部的に別のコントローラーへフォワードさせることも簡単にできます。\
これには :method:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller::forward` メソッドを使用します。\
ブラウザにリダイレクトさせるのではなく、内部的なサブリクエストを作成し、\
指定されたコントローラーを呼び出します。\
``forward()`` メソッドは、その呼び出されたコントローラーが作成する ``Resopnse`` オブジェクトを返します。

.. code-block:: php

    public function indexAction($name)
    {
        $response = $this->forward('AcmeHelloBundle:Hello:fancy', array(
            'name'  => $name,
            'color' => 'green',
        ));

        // ... レスポンスをさらに加工するか、直接返す

        return $response;
    }

`forward()` メソッドは、ルーティングの設定で使用したものと同じコントローラー表現を使っていることに注意してください。\
この場合であれば、ターゲットとなるコントローラークラスは、\ ``AcmeHelloBundle`` 内の ``HelloController`` となるでしょう。\
このメソッドに渡される配列は、ターゲットコントローラーの引数になります。\
同じインターフェースが、テンプレートにコントローラーをエンベッドするときにも使われます (:ref:`templating-embedding-controller`\ 参照)。\
ターゲットとなるコントローラーは次のようになります。

.. code-block:: php

    public function fancyAction($name, $color)
    {
        // ... Response オブジェクトの作成をして返す
    }

ルートに対してコントローラーを作成する場合と同様に、\ ``fancyAction`` へ渡す引数の順番は問題ではありません。\
Symfony2 は、キー(``name``) をメソッドの引数名(``$name``)とマッチさせます。\
引数の順番を変更した場合も、Symfony2 が適切な変数に引き渡してくれます。　

.. tip::

    基底 ``Controller`` の他のメソッドと同様に、\ ``forward`` メソッドは Symfony2 コア機能へのショートカットに過ぎません。\
    フォワードでは、現在のリクエストが複製されます。
    :ref:`サブリクエスト <http-kernel-sub-requests>`\  が ``http_kernel`` サービスで実行されると、\ ``HttpKernel`` は ``Response`` オブジェクトを返します。
    
    .. code-block:: php

        use Symfony\Component\HttpKernel\HttpKernelInterface;

        $path = array(
            '_controller' => 'AcmeHelloBundle:Hello:fancy',
            'name'        => $name,
            'color'       => 'green',
        );
        $request = $this->container->get('request');
        $subRequest = $request->duplicate(array(), null, $path);

        $httpKernel = $this->container->get('http_kernel');
        $response = $httpKernel->handle($subRequest, HttpKernelInterface::SUB_REQUEST);

.. index::
   single: Controller; Rendering templates

.. _controller-rendering-templates:

テンプレートのレンダリング
~~~~~~~~~~~~~~~~~~~~~~~~~~

必須条件ではないとしても、ほとんどのコントローラーでは HTML (もしくはその他のフォーマット) を生成するテンプレートのレンダリングを最終的に行うことになるでしょう。\
``renderView()`` メソッドは、テンプレートをレンダリングし、コンテンツを返します。\
テンプレートからできたそのコンテンツは、\ ``Response`` オブジェクトの作成に使用されます。

.. code-block:: php

    use Symfony\Component\HttpFoundation\Response;

    $content = $this->renderView(
        'AcmeHelloBundle:Hello:index.html.twig',
        array('name' => $name)
    );

    return new Response($content);

``render()`` メソッドを使用すれば、これを 1 ステップで行うこともできます。\
このメソッドは、テンプレートからできたコンテンツを内包している ``Response`` オブジェクトを返します。

.. code-block:: php

    return $this->render(
        'AcmeHelloBundle:Hello:index.html.twig',
        array('name' => $name)
    );

どちらの場合でも、\ ``AcmeHelloBundle`` 内の ``Resources/views/Hello/index.html.twig`` というテンプレートがレンダリングされます。

Symfony のテンプレートエンジンについては\ :doc:`テンプレート </book/templating>`\ 章で詳しく説明しています。

.. tip::

    ``@Template`` アノテーションを使うと ``render`` メソッド呼び出しの記述を省略できます。
    詳細は :doc:`FrameworkExtraBundle のドキュメント </bundles/SensioFrameworkExtraBundle/annotations/view>`\ を参照してください。

.. tip::

    ``renderView`` メソッドは、\ ``templating`` サービスを使用するショートカットです。\
    ``templating`` サービスは、直接利用することもできます。

    .. code-block:: php

        $templating = $this->get('templating');
        $content = $templating->render(
            'AcmeHelloBundle:Hello:index.html.twig',
            array('name' => $name)
        );

.. note::

    サブディレクトリにあるテンプレートをレンダリングすることもできます。
    ディレクトリ構造はコントローラ名を指定する部分に記述することに注意してください。

    .. code-block:: php

        $templating->render(
            'AcmeHelloBundle:Hello/Greetings:index.html.twig',
            array('name' => $name)
        );
        // Resources/views/Hello/Greetings にある index.html.twig がレンダリングされる

.. index::
   single: Controller; Accessing services

他のサービスへのアクセス
~~~~~~~~~~~~~~~~~~~~~~~~

基底コントローラークラスを継承した場合は、\ ``get()`` メソッドを使用して、\
あらゆる Symfony2 サービスへのアクセスを行うことができます。\
一般的なサービスとしては、次のようなサービスが必要となるかもしれません。

.. code-block:: php

    $templating = $this->get('templating');

    $router = $this->get('router');

    $mailer = $this->get('mailer');

サービスは無数に存在しており、自分で定義することも自由になっています。\
利用可能な全てのサービスを列挙するには、コンソールコマンドの ``container:debug`` を使用してください。

.. code-block:: bash

    $ php app/console container:debug

詳細は :doc:`/book/service_container` 章を参照してください。

.. index::
   single: Controller; Managing errors
   single: Controller; 404 pages

エラーと 404
------------

"not found" になった場合は、HTTP プロトコルに沿うように 404 レスポンスを返すべきでしょう。\
このためには専用の例外をスローします。\
基底コントローラークラスを継承している場合は次のようにします。

.. code-block:: php

    public function indexAction()
    {
        // データベースからオブジェクトを取得する
        $product = ...;
        if (!$product) {
            throw $this->createNotFoundException('製品が存在しません');
        }

        return $this->render(...);
    }

``createNotFoundException()`` メソッドは ``NotFoundHttpException`` オブジェクトを作成します。\
このオブジェクトは、Symfony 内部で最終的に 404 HTTP レスポンスを引き起こすことになります。

もちろん、コントローラー内ではどんな ``Exception`` クラスをスローしても問題ありません。\
Symfony2 は、自動的に 500 HTTP レスポンスコードを返します。

.. code-block:: php

    throw new \Exception('Something went wrong!');

どの場合においても、エンドユーザーにはスタイルの整ったエラーページが表示され、\
(デバッグモードで見ている場合は)開発者にデバッグエラーページが表示されます。\
これらのエラーページは、両方ともカスタマイズが可能です。\
詳細は、クックブックの":doc:`/cookbook/controller/error_pages`"レシピを参照してください。

.. index::
   single: Controller; The session
   single: Session

セッション管理
--------------

Symfony2 は、使いやすいセッションオブジェクトを提供しています。\
セッションオブジェクトを使って、リクエスト間でユーザー (ブラウザを使用しているユーザーや、ボット、WEB サービスでも) の情報を保存しておくことができます。\
デフォルトでは、Symfony2 はネイティブな PHP セッションを使用して、属性をクッキーに保存します。

セッションのストアと取得は、コントローラー内で容易に行うことができます。

.. code-block:: php

    use Symfony\Component\HttpFoundation\Request;

    public function indexAction(Request $request)
    {
        $session = $request->getSession();

        // 後のユーザーリクエストで使うために属性を保存する
        $session->set('foo', 'bar');

        // 別のコントローラーの、別のリクエストにて
        $foo = $session->get('foo');

        // キーが存在しない場合にデフォルト値を使う
        $filters = $session->get('filters', array());
    }

これらの属性は、ユーザーのセッションが存続する間は保持されます。

.. index::
   single: Session; Flash messages

フラッシュメッセージ
~~~~~~~~~~~~~~~~~~~~

ユーザーの次のリクエストまでの間のみセッションに保持されるような、小さなメッセージを保存することもできます。\
これは、フォームを処理しているときに便利です。\
リクエストをリダイレクトし、\ *次の*\ リクエストで特別なメッセージを表示させたい時に使います。\
この種のメッセージは「フラッシュ」メッセージと呼ばれています。

フォームの送信を処理する場合を考えてみましょう。

.. code-block:: php

    use Symfony\Component\HttpFoundation\Request;

    public function updateAction(Request $request)
    {
        $form = $this->createForm(...);

        $form->handleRequest($request);

        if ($form->isValid()) {
            // 何か処理をする

            $this->get('session')->getFlashBag()->add(
                'notice',
                '変更が保存されました。'
            );

            return $this->redirect($this->generateUrl(...));
        }

        return $this->render(...);
    }

リクエストの処理後、コントローラーは ``notice`` というフラッシュメッセージをセットし、リダイレクトさせます。\
この名前 (``notice``) は重要ではなく、自分自身でメッセージの種類が特定できるものであれば問題ありません。

次に実行されるアクションのテンプレートで、\ ``notice`` メッセージをレンダリングするには、次のようなコードになります。

.. configuration-block::

    .. code-block:: html+jinja

        {% for flashMessage in app.session.flashbag.get('notice') %}
            <div class="flash-notice">
                {{ flashMessage }}
            </div>
        {% endfor %}

    .. code-block:: php

        <?php foreach ($view['session']->getFlash('notice') as $message): ?>
            <div class="flash-notice">
                <?php echo "<div class='flash-error'>$message</div>" ?>
            </div>
        <?php endforeach; ?>

設計的には、フラッシュメッセージは、ただ 1 回だけのリクエスト間でのみ生存するように意図されています(「瞬く(flash)間に消える」)。\
まさにこの例で示したような、リダイレクトをまたがる時において使われるように設計されているのです。

.. index::
   single: Controller; Response object

レスポンスオブジェクト
----------------------

コントローラーが満たさなければいけないただ 1 つの要件は、\ ``Response`` オブジェクトを返すことです。\
:class:`Symfony\\Component\\HttpFoundation\\Response` クラスは、\
HTTP レスポンス (HTTP ヘッダーのテキストメッセージとクライアントに返されるべきコンテンツ) を、PHP によって抽象化しているクラスです。

.. code-block:: php

    use Symfony\Component\HttpFoundation\Response;

    // ステータスコード 200(デフォルト)の Response を作成
    $response = new Response('Hello '.$name, Response::HTTP_OK);

    // ステータスコード 200 の JSON レスポンスを作成
    $response = new Response(json_encode(array('name' => $name)));
    $response->headers->set('Content-Type', 'application/json');

.. versionadded:: 2.4
    HTTP ステータスコード定数は、Symfony 2.4 以降で利用可能です。

.. tip::

    ``headers`` プロパティは、\ :class:`Symfony\\Component\\HttpFoundation\\HeaderBag` オブジェクトとなります。\
    このオブジェクトには ``Resopnse`` のヘッダの読み込みと変更のための便利なメソッドがついています。\
    ヘッダ名は正規化されるので、\ ``Content-Type`` は ``content-type`` や、さらに言えば ``content_type`` でも同等に使用できます。

.. tip::

    特定のレスポンスを手軽に返すのに有用なクラスも用意されています。

    - JSON 用 :class:`Symfony\\Component\\HttpFoundation\\JsonResponse`
      :ref:`component-http-foundation-json-response` を参照
    - ファイル用 :class:`Symfony\\Component\\HttpFoundation\\BinaryFileResponse`
      :ref:`component-http-foundation-serving-files` を参照

.. index::
   single: Controller; Request object

リクエストオブジェクト
----------------------

ルーティングのプレースホルダー値もそうですが、\
基底 ``Controller`` クラスを継承している場合、\ ``Request`` オブジェクトへのアクセスも可能です。
コントローラーの引数に `Symfony\Component\HttpFoundation\Request` クラスのタイプヒントが指定されていれば、フレームワークにより自動的に ``Request`` オブジェクトが注入されます。

.. code-block:: php

    use Symfony\Component\HttpFoundation\Request;

    public function indexAction(Request $request)
    {
        $request->isXmlHttpRequest(); // Ajax リクエストかどうか？

        $request->getPreferredLanguage(array('en', 'fr'));

        $request->query->get('page'); // $_GET パラメーターを取得

        $request->request->get('page'); // $_POST パラメーターを取得
    }

``Response`` オブジェクトと同様に、\ ``HeaderBag`` オブジェクト内にリクエストヘッダーの値が保存されており、\
容易にアクセスできます。

まとめ
------

ページを作成するのであれば、最終的にはそのページに対するロジックの入ったコードが必要となるでしょう。\
Symfony では、これをコントローラーと呼んでいます。\
コントローラーは、ユーザーに返される最終的な ``Response`` オブジェクトを返すために必要なことがなんでもできる PHP 関数です。

簡単にコントローラーを実装するために、基底 ``Controller`` クラスを継承することもできます。\
この基底クラスには、コントローラーでよく利用される処理のショートカットメソッドが多数用意されています。\
例えば、コントローラー内に HTML コードを書きたくはないでしょうから、\ ``render()`` メソッドを使って、\
テンプレートからコンテンツをレンダリングし、返してもらうことができます。

データベースへの永続化やその取得、フォームの送信、キャッシュといったコントローラーの使い方は、他の章で説明しています。

クックブックでより深く
----------------------

* :doc:`/cookbook/controller/error_pages`
* :doc:`/cookbook/controller/service`

.. 2011/09/11 gilbite  07d55eff273cfc4cc4cd9a40352bf5e9d55965bb
.. 2014/01/01 hidenorigoto 69f8a8831d019a8bf61daa8b54310955352df69c
