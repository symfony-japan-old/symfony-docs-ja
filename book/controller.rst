.. index::
   single: Controller

コントローラ
============

コントローラは、HTTP リクエストから情報を取得し、\
(Symfony2 の ``Response`` オブジェクトとして) HTTP レスポンスを作成して返す PHP 関数です。\
レスポンスは、HTML ページ、XML ドキュメント、シリアライズされた JSON 配列、\
画像、リダイレクト、404 エラー、その他思いつく物なんでもよいでしょう。\
コントローラには、\ *あなたのアプリケーション*\ が、ページコンテンツをレンダリングする際に必要な任意のロジックが盛り込まれます。

このことがどれだけシンプルなことなのか？\
Symfony2 コントローラが実際に動作しているところを見ていきましょう！\
このコントローラは、\ ``Hello world!`` と出力されるページをレンダリングする例です。\ ::

    use Symfony\Component\HttpFoundation\Response;

    public function helloAction()
    {
        return new Response('Hello world!');
    }

コントローラの目指すところは常に同じです。\
すなわち、\ ``Response`` オブジェクトを作成して返すことです。\
そこに至るまでに、リクエストからの情報の読み込みや、データベースリソースの読み込み、\
メールの送信、ユーザのセッションへの情報の設定を行うかもしれませんが、\
どの場合においても、コントローラは最終的にはクライアントに届けることになる ``Response`` オブジェクトを返します。

心配しなきゃいけないような魔法やその他要件はありません！\
いくつかよくある例を見てみましょう。

* *コントローラA* サイトのトップページコンテンツである ``Response`` オブジェクトを準備する。

* *コントローラB* リクエストから ``slug`` パラメータを読み込み、\
  データベースからブログエントリを取得するためにそれを使用し、\
  そのブログを表示する ``Response`` オブジェクトを作成する。\
  もし ``slug`` がデータベース中に見つからなければ、ステータスコード 404 の ``Response`` オブジェクトを作成します。

* *コントローラC* お問い合わせフォームの送信を扱う。\
  リクエストからフォームの情報を読み込み、データベースにセーブし、\
  ウェブマスターにその情報をメールします。\
  最終的には、クライアントのブラウザをお礼ページにリダイレクトさせる ``Response`` オブジェクトを作成します。

.. index::
   single: Controller; Request-controller-response lifecycle

リクエスト、コントローラ、レスポンスのライフサイクル
----------------------------------------------------

Symfony2 プロジェクトによって処理されるリクエストは、全て同じシンプルなライフサイクルに従っています。\
フレームワークが、なんども繰り返されるようなタスクを解決し、\
あなたの作成したコードが格納されているコントローラを実行します。

#. 各リクエストは、単一のフロントコントローラファイル(``app.php`` や ``app_dev.php``)によって処理され、アプリケーションがブートされます。

#. ``Router`` が、リクエストから情報(URI)を読み込み、その情報にマッチするルートを見つけ、そのルートの ``_controller`` パラメータを読み込みます。

#. マッチしたルートのコントローラが実行され、コントローラ内のコードにより ``Response`` オブジェクトが作成され、返されます。

#. HTTP ヘッダと ``Response`` オブジェクトのコンテンツがクライアントに送り返されます。

.. Creating a page is as easy as creating a controller (#3) and making a route that maps a URL to that controller (#2).

ページを作成するということはとても簡単で、コントローラの作成(#3)を行い、URL をそのコントローラにマップするルートの作成(#2)を行うということになります。

.. note::

    似たような名前ですが、「フロントコントローラ」は、この章で話す「コントローラ」とは別ものです。\
    フロントコントローラは、小さな PHP ファイルで公開領域に配置されており、全てのリクエストが通過します。\
    典型的なアプリケーションでは、プロダクション用のフロントコントローラ(``app.php``)と、\
    開発用のフロントコントローラ(``app_dev.php``)が存在するでしょう。\
    このファイルを編集したり参照したり思い悩むことはおそらく無いでしょう。

.. index::
   single: Controller; Simple example

シンプルなコントローラ
----------------------

コントローラは、PHP callable(関数、オブジェクトのメソッド、\ ``Clusure``)であれば良いのですが、\
特に Symfony2 では、コントローラオブジェクト内の1つのメソッドのことを指します。\
コントローラは、\ *アクション*\ とも呼びます。

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

    *コントローラ*\ は ``indexAction`` メソッドであり、\ *コントローラクラス*\(``HelloController``) 内に存在していることに注意してください。\
    混乱しないでくださいね。\ *コントローラクラス*\ と呼ぶのは、単に複数のコントローラ/アクションをグループ化するのに便利だからです。\
    典型的にはコントローラクラスは複数のコントローラ/アクションを内包することになるでしょう(``updateAction`` や ``deleteAction`` など)。


このコントローラはとても単純ではあるのですが、一つずつ見ていきましょう。

* *3行目*: Symfony2 は PHP 5.3 の名前空間機能をうまく利用して、コントローラクラス全体を名前空間付けしています。\
  ``use`` キーワードで、コントローラが返すべき ``Response`` クラスをインポートしています。

* *6行目*: クラス名は、そのコントローラの名前(``Hello``)と ``Controller`` という文字列の結合です。\
  これは、コントローラ群に一貫性を提供し、ルーティング設定の際に最初の部分(``Hello``)のみの参照で済むようにするための慣習です。

* *8行目*: コントローラクラス内の各アクションは、サフィックスとして ``Action`` が付けられています。\
  ルーティングの設定では、アクション名(``index``)で参照されます。\
  次節では、ルート(URI をアクションにマッピングする)を作成しますが、\
  そのルートのプレースホルダ(``{name}``)が、どうやってアクションメソッドの引数(``$name``)になっていくのかを見ていきます。

* *10行目*: コントローラが ``Response`` オブジェクトを作成して返します。

.. index::
   single: Controller; Routes and controllers

URL をコントローラにマッピングする
----------------------------------

先程のコントローラはシンプルな HTML ページを返します。\
ですが、実際にブラウザで確認するには、ルートを作成する必要があります。\
ルートは、特定の URL パターンをコントローラにマッピングするものです。

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        hello:
            pattern:      /hello/{name}
            defaults:     { _controller: AcmeHelloBundle:Hello:index }

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <route id="hello" pattern="/hello/{name}">
            <default key="_controller">AcmeHelloBundle:Hello:index</default>
        </route>

    .. code-block:: php

        // app/config/routing.php
        $collection->add('hello', new Route('/hello/{name}', array(
            '_controller' => 'AcmeHelloBundle:Hello:index',
        )));

これで、\ ``/hello/ryan`` を見に行くと、\ ``HelloController::indexAction()`` コントローラが実行され、\
``$name`` 変数として\ ``ryan`` が渡されるようになります。\
「ページ」を作成するということは、単純にコントローラメソッドを作成し、ルートと関連付けることを意味するのです。

コントローラを指定する構文 ``AcmeHelloBundle:Hello:index`` に注意してください。\
Symfony2 はコントローラを指定するために、柔軟な文字列記法を使用しています。\
この例は最も一般的な構文で、Symfony2 に ``AcmeHelloBundle`` という名前のバンドルに存在している、\ ``HelloController``  というクラスを探すように伝えています。\
そして、\ ``indexAction()`` メソッドが実行されます。

コントローラを指定する文字列フォーマットについて、詳細を知りたい場合は :ref:`controller-string-syntax` を参照してください。


.. note::

    この例では、ルーティングの設定ファイルを ``app/config/`` に直においていますが、\
    ルートの構成としては、各ルートは自分が属しているバンドル内に置くほうが良い方法です。\
    詳細は :ref:`routing-include-external-resources` を参照してください。


.. tip::

    ルーティングに関してもっと学びたい場合は、\ :doc:`ルーティング</book/routing>` を参照してください。

.. index::
   single: Controller; Controller arguments

.. _route-parameters-controller-arguments:

ルートパラメータとコントローラの引数の関係
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

``_controller`` パラメータである ``AcmeHelloBundle:Hello:index`` が、\ ``AcmeHelloBundle`` 内の ``HelloController::indexAction()`` メソッドを指定していることはわかりましたね。\
もっとおもしろいのは、そのメソッドに渡される引数の話です。

.. code-block:: php

    <?php
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

このコントローラはただ1つの引数 ``$name`` を持っており、この引数は、マッチしたルートの ``{name}`` に対応しています(今回の例では ``ryan``)。\
実際には、コントローラが実行されるとき、Symfony2 はコントローラの各引数とルートのパラメータをマッチさせています。\
次の例を見てください。

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        hello:
            pattern:      /hello/{first_name}/{last_name}
            defaults:     { _controller: AcmeHelloBundle:Hello:index, color: green }

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <route id="hello" pattern="/hello/{first_name}/{last_name}">
            <default key="_controller">AcmeHelloBundle:Hello:index</default>
            <default key="color">green</default>
        </route>

    .. code-block:: php

        // app/config/routing.php
        $collection->add('hello', new Route('/hello/{first_name}/{last_name}', array(
            '_controller' => 'AcmeHelloBundle:Hello:index',
            'color'       => 'green',
        )));

このルート用のコントローラは、複数の引数を持つことができます。\ ::

    public function indexAction($first_name, $last_name, $color)
    {
        // ...
    }

プレースホルダ変数 (``{first_name}``, ``{last_name}``) もそうですが、\
dafault の ``color`` 変数も、コントローラの引数として有効です。\
ルートがマッチした際に、プレースホルダの変数が、\ ``defaults`` と共にマージされ、\
コントローラ内で使用できるように1つの配列として作成されます。\

ルートパラメータをコントローラの引数にマッピングするのはとても簡単で柔軟性があります。\
開発時に下記のガイドラインを心に留めておいてください。

* **コントローラの引数の順番は関係ない**

    Symfony は、ルートのパラメータ名を、コントローラメソッドの表記にある変数名にマッチさせることができます。\
    言い換えると、\ ``{last_name}`` パラメータは ``$last_name`` にマッチするということです。\
    コントローラの引数の順は、全く異なる順番でもうまく動きます。\ ::

        public function indexAction($last_name, $color, $first_name)
        {
            // ..
        }

.. * **Each required controller argument must match up with a routing parameter**

* **必須な引数は絶対にルーティングパラメータとマッチしないといけない**

    次のような例では ``RuntimeException`` が投げられます。\
    ルートに ``foo`` パラメータが定義されていないからです。\ ::

        public function indexAction($first_name, $last_name, $color, $foo)
        {
            // ..
        }

    ただし、オプションにしてしまえば全く問題ありません。\
    次の例では例外は投げられません。\ ::

        public function indexAction($first_name, $last_name, $color, $foo = 'bar')
        {
            // ..
        }

* **全てのルーティングパラメータがコントローラ引数になっていないといけないわけじゃない**

    例えば、\ ``last_name`` パラメータがコントローラにとって重要でないのであれば、完全に省略してしまっても大丈夫です。\ ::

        public function indexAction($first_name, $color)
        {
            // ..
        }

.. tip::

    全てのルートは特別なパラメータである ``_route`` を持っています。\
    このパラメータは、マッチしたルートの名前(``hello``)を意味します。\
    いつも便利かというとそうでもありませんが、これもコントローラの引数として同様に有効です。

コントローラ引数としての ``Request`` 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

簡単にするため、コントローラに引数として ``Request`` オブジェクトを渡すようにすることも可能です。\
特にフォームを扱っている場合に便利です。次の例を見てください。\ ::

    use Symfony\Component\HttpFoundation\Request;

    public function updateAction(Request $request)
    {
        $form = $this->createForm(...);
        
        $form->bindRequest($request);
        // ...
    }

.. index::
   single: Controller; Base controller class

基底コントローラクラス
----------------------

Symfony2 には、基底 ``Controller`` クラスが用意されています。\
このクラスは、一般的なコントローラタスクを補助してくれたり、\
必要となるであろうあらゆるリソースへのアクセスを提供してくれます。\
この ``Controller`` クラスを継承することで、いくつかのヘルパメソッドを有効利用することができます。

.. ここおかしいきがする
.. Add the ``use`` statement atop the ``Controller`` class and then modify the
.. ``HelloController`` to extend it:

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

この時点では特にコントローラの処理が変わったわけではありません。\
次節では、基底コントローラクラスの存在により使用可能となるヘルパメソッドを見ていきます。\
これらのメソッドは、単に Symfony2 の機能へのショートカットです。\
その機能自体は、基底 ``Controller`` クラスを通しても通さなくても利用可能です。\
実際のコア機能を参照するには、\ :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller` を見るとよいでしょう。

.. tip::

    Symfony では、この基底クラスを継承するのは *オプション*\ です。\
    たしかに便利なショートカットはありますが、強制ではありません。\
    また、\ ``Symfony\Component\DependencyInjection\ContainerAware`` を継承することもできます。\
    サービスコンテナオブジェクトへは、\ ``container`` プロパティを通してアクセス可能になります。

.. note::

    :doc:`コントローラをサービスとして</cookbook/controller/service>`\ 定義することも可能です。

.. index::
   single: Controller; Common Tasks

一般的なコントローラタスク
--------------------------

コントローラが実質的になんでもできるとは言っても、ほとんどのコントローラでは、\
同じ基礎的なタスクを何度も何度も行うことになるでしょう。\
Symfony2 では、リダイレクトやフォワーディング、テンプレートのレンダリング、コアサービスへのアクセスといったことを、\
とても簡単に扱うことができます。

.. index::
   single: Controller; Redirecting

リダイレクト
~~~~~~~~~~~~

ユーザを別のページにリダイレクトさせたいときは、\ ``redirect()`` メソッドを使用します。\ ::

    public function indexAction()
    {
        return $this->redirect($this->generateUrl('homepage'));
    }

``generateUrl()`` メソッドは、与えられたルートに対する URL を生成するヘルパ関数です。\
詳細な情報は、\ :doc:`ルーティング </book/routing>` 章を参照してください。

デフォルトでは、\ ``redirect()`` メソッドは 302(temporary) リダイレクトとして動作します。\
301(permanent) としてリダイレクトさせるには、第二引数を変更します\ ::

    public function indexAction()
    {
        return $this->redirect($this->generateUrl('homepage'), 301);
    }

.. tip::

    ``redirect()`` メソッドは、ユーザをリダイレクトさせることに特化した ``Resoponse`` オブジェクトの作成へのショートカットです。\
    これは、次のコードと同等です。

    .. code-block:: php

        use Symfony\Component\HttpFoundation\RedirectResponse;

        return new RedirectResponse($this->generateUrl('homepage'));

.. index::
   single: Controller; Forwarding

フォワーディング
~~~~~~~~~~~~~~~~

内部的に別のコントローラへフォワードさせることも簡単にできます。\
これには ``forward()`` メソッドを使用します。\
ブラウザにリダイレクトさせるのではなく、内部的なサブリクエストを作成し、\
指定されたコントローラを呼び出します。\
``forward()`` メソッドは、その呼び出されたコントローラが作成する ``Resopnse`` オブジェクトを返します。\ ::

    public function indexAction($name)
    {
        $response = $this->forward('AcmeHelloBundle:Hello:fancy', array(
            'name'  => $name,
            'color' => 'green'
        ));

        // further modify the response or return it directly
        
        return $response;
    }

`forward()` メソッドは、ルーティングの設定で使用したものと同じコントローラ表現を使っていることに注意してください。\
この場合であれば、ターゲットとなるコントローラクラスは、\ ``AcmeHelloBundle`` 内の ``HelloController`` となるでしょう。\
このメソッドに渡される配列は、ターゲットコントローラの引数になります。\
同じインターフェースが、テンプレートにコントローラをエンベッドするときにも使われます(:ref:`templating-embedding-controller`\ 参照)。\
ターゲットとなるコントローラは次のようになります。\ ::

    public function fancyAction($name, $color)
    {
        // ... Response オブジェクトの作成をして返す
    }

ルートに対してコントローラを作成する場合と同様に、\ ``fancyAction`` へ渡す引数の順番は問題ではありません。\
Symfony2 は、キー(``name``) をメソッドの引数名(``$name``)とマッチさせます。\
引数の順番を変更した場合も、Symfony2 が適切な変数に引き渡してくれます。　

.. tip::

    基底 ``Controller`` の他のメソッドと同様に、\ ``forward`` メソッドは Symfony2 コア機能へのショートカットに過ぎません。\
    フォワーディングは ``http_kernel`` サービスを通じて直接的に行うことができます。\
    フォワーディングは ``Resopnse`` オブジェクトを返します。\ ::
    
        $httpKernel = $this->container->get('http_kernel');
        $response = $httpKernel->forward('AcmeHelloBundle:Hello:fancy', array(
            'name'  => $name,
            'color' => 'green',
        ));

.. index::
   single: Controller; Rendering templates

.. _controller-rendering-templates:

テンプレートのレンダリング
~~~~~~~~~~~~~~~~~~~~~~~~~~

必須条件ではないとしても、ほとんどのコントローラでは、\
HTML(もしくはその他のフォーマット)を生成するテンプレートのレンダリングを最終的に行うことになるでしょう。\
``renderView()`` メソッドは、テンプレートをレンダリングし、コンテンツを返します。\
テンプレートからできたそのコンテンツは、\ ``Response`` オブジェクトの作成に使用されます。\ ::

    $content = $this->renderView('AcmeHelloBundle:Hello:index.html.twig', array('name' => $name));

    return new Response($content);

``render()`` メソッドを使用すれば、これを1ステップで行うこともできます。\
このメソッドは、テンプレートからできたコンテンツを内包している ``Response`` オブジェクトを返します。\ ::

    return $this->render('AcmeHelloBundle:Hello:index.html.twig', array('name' => $name));

どちらの場合でも、\ ``AcmeHelloBundle`` 内の ``Resources/views/Hello/index.html.twig`` というテンプレートがレンダリングされます。

Symfony のテンプレートエンジンについては\ :doc:`テンプレート </book/templating>`\ 章で詳しく説明しています。

.. tip::

    ``renderView`` メソッドは、\ ``templating`` サービスを使用するショートカットです。\
    ``templating`` サービスは、直接利用することもできます。\ ::
    
        $templating = $this->get('templating');
        $content = $templating->render('AcmeHelloBundle:Hello:index.html.twig', array('name' => $name));

.. index::
   single: Controller; Accessing services

他のサービスへのアクセス
~~~~~~~~~~~~~~~~~~~~~~~~

基底コントローラクラスを継承した場合は、\ ``get()`` メソッドを使用して、\
あらゆる Symfony2 サービスへのアクセスを行うことができます。\
一般的なサービスとしては、次のようなサービスが必要となるかもしれません。\ ::

    $request = $this->getRequest();

    $response = $this->get('response');

    $templating = $this->get('templating');

    $router = $this->get('router');

    $mailer = $this->get('mailer');

サービスは無数に存在しており、自分で定義することも自由になっています。\
利用可能な全てのサービスを列挙するには、コンソールコマンドの ``container:debug`` を使用してください。

.. code-block:: bash

    php app/console container:debug

詳細は :doc:`/book/service_container` 章を参照してください。

.. index::
   single: Controller; Managing errors
   single: Controller; 404 pages

エラーと404
-----------

"not found" になった場合は、HTTP プロトコルに沿うように 404 レスポンスを返すべきでしょう。\
このためには専用の例外を投げます。\
基底コントローラクラスを継承している場合は次のようにします。\ ::

    public function indexAction()
    {
        $product = // retrieve the object from database
        if (!$product) {
            throw $this->createNotFoundException('The product does not exist');
        }

        return $this->render(...);
    }

``createNotFoundException()`` メソッドは ``NotFoundHttpException`` オブジェクトを作成します。\
このオブジェクトは、Symfony 内部で最終的に 404 HTTP レスポンスを引き起こすことになります。

もちろん、コントローラ内ではどんな ``Exception`` クラスを投げても問題ありません。\
Symfony2 は、自動的に 500 HTTP レスポンスコードを返します。

.. code-block:: php

    throw new \Exception('Something went wrong!');

どの場合においても、エンドユーザにはスタイルの整ったエラーページが表示され、\
(デバッグモードで見ている場合は)開発者にデバッグエラーページが表示されます。\
これらのエラーページは、両方ともカスタマイズが可能です。\
詳細は、クックブックの":doc:`/cookbook/controller/error_pages`"レシピを参照してください。

.. index::
   single: Controller; The session
   single: Session

セッション管理
--------------

Symfony2 は、ナイスなセッションオブジェクトを提供しています。\
セッションオブジェクトを使って、リクエスト間でユーザ(ブラウザを使用しているユーザや、ボット、WEB サービスでも)の情報をストアしておくことができます。\
デフォルトでは、Symfony2 はネイティブな PHP セッションを使用して、アトリビュートをクッキーにストアします。

セッションのストアと取得は、コントローラ内で容易に行うことができます。\ ::

    $session = $this->getRequest()->getSession();

    // 同じユーザの今後のリクエストで再使用するためにアトリビュートをストアする
    $session->set('foo', 'bar');

    // 上とは別のリクエストで別のコントローラで
    $foo = $session->get('foo');

    // ユーザのロケールをセット
    $session->setLocale('fr');

これらのアトリビュートは、ユーザのセッションが残っている間は、生き続けます。

.. index::
   single Session; Flash messages

フラッシュメッセージ
~~~~~~~~~~~~~~~~~~~~

そのユーザの次のリクエストまで、その間でだけセッション上にストアされるような、小さなメッセージをストアすることもできます。\
これは、フォームを処理しているときに便利です。\
リダイレクトさせて、\ *次の*\ リクエストで特別なメッセージを表示させたい時です。\
この種のメッセージは「フラッシュ」メッセージと呼ばれています。

フォームのサブミットを処理する場合を考えてみましょう。\ ::

    public function updateAction()
    {
        $form = $this->createForm(...);

        $form->bindRequest($this->getRequest());
        if ($form->isValid()) {
            // 何か処理をする

            $this->get('session')->setFlash('notice', '変更が保存されました!');

            return $this->redirect($this->generateUrl(...));
        }

        return $this->render(...);
    }

リクエストの処理後、コントローラは ``notice`` というフラッシュメッセージをセットし、リダイレクトさせます。\
この名前(``notice``)は重要ではなく、自分自身でメッセージの種類が特定できるものであれば問題ありません。

次に実行されるアクションのテンプレートで、\ ``notice`` メッセージをレンダリングするには、次のようなコードになります。

.. configuration-block::

    .. code-block:: html+jinja

        {% if app.session.hasFlash('notice') %}
            <div class="flash-notice">
                {{ app.session.flash('notice') }}
            </div>
        {% endif %}

    .. code-block:: php
    
        <?php if ($view['session']->hasFlash('notice')): ?>
            <div class="flash-notice">
                <?php echo $view['session']->getFlash('notice') ?>
            </div>
        <?php endif; ?>

設計的には、フラッシュメッセージは、ただ1回だけのリクエスト間でのみ生存するように意図されています(「瞬く(flash)間に消える」)。\
まさにこの例で示したような、リダイレクトをまたがる時において使われるように設計されているのです。

.. index::
   single: Controller; Response object

レスポンスオブジェクト
----------------------

コントローラが満たさなければいけないただ1つの要件は、\ ``Response`` オブジェクトを返すことです。\
:class:`Symfony\\Component\\HttpFoundation\\Response` クラスは、\
HTTP レスポンス(HTTP ヘッダーのテキストメッセージとクライアントに返されるべきコンテンツ)を、PHP によって抽象化しているクラスです。\ ::

    // ステータスコード 200(デフォルト)の Response を作成
    $response = new Response('Hello '.$name, 200);
    
    // ステータスコード 200 の JSON レスポンスをを作成
    $response = new Response(json_encode(array('name' => $name)));
    $response->headers->set('Content-Type', 'application/json');

.. tip::

    ``headers`` プロパティは、\ :class:`Symfony\\Component\\HttpFoundation\\HeaderBag` オブジェクトとなります。\
    このオブジェクトには ``Resopnse`` のヘッダの読み込みと変更のための便利なメソッドがついています。\
    ヘッダ名は正規化されるので、\ ``Content-Type`` は ``content-type`` や、さらに言えば ``content_type`` でも同等に使用できます。

.. index::
   single: Controller; Request object

リクエストオブジェクト
----------------------

ルーティングのプレースホルダ値もそうですが、\
基底 ``Controller`` クラスを継承している場合、\ ``Request`` オブジェクトへのアクセスも可能です。\ ::

    $request = $this->getRequest();

    $request->isXmlHttpRequest(); // Ajax リクエストかどうか

    $request->getPreferredLanguage(array('en', 'fr'));

    $request->query->get('page'); // $_GET パラメータを取得

    $request->request->get('page'); // $_POST パラメータを取得

``Response`` オブジェクトと同様に、\ ``HeaderBag`` オブジェクト内にリクエストヘッダがストアされており、\
容易にアクセスが可能です。

Final Thoughts
--------------

ページを作成するのであれば、最終的にはそのページに対するロジックの入ったコードが必要となるでしょう。\
Symfony では、これをコントローラと呼んでいます。\
コントローラは、ユーザに返される最終的な ``Response`` オブジェクトを返すために必要なことがなんでもできる PHP 関数です。

簡単のために、基底 ``Controller`` クラスを継承することもできます。\
この基底クラスは、多くの一般的なコントローラタスクへのショートカットメソッドを備えています。\
例えば、コントローラ内に HTML コードを書きたくはないでしょうから、\ ``render()`` メソッドを使って、\
テンプレートからコンテンツをレンダリングし、返してもらうことができます。

他の章では、データベースへの永続化やその習得、フォームのサブミット、キャッシュ等の、\
コントローラの使い方を説明しています。

クックブックでより深く
----------------------

* :doc:`/cookbook/controller/error_pages`
* :doc:`/cookbook/controller/service`

.. 2011/08/28 hidenorigoto 07d55eff273cfc4cc4cd9a40352bf5e9d55965bb（タイトル翻訳のみ）
.. 2011/09/11 gilbite  07d55eff273cfc4cc4cd9a40352bf5e9d55965bb 
