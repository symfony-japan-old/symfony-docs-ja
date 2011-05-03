.. 2011/05/02 hidenorigoto 310854fe

コントローラ
============

チュートリアルの 3 つめのステップとして、この章ではコントローラの機能をさらに見ていきます。

フォーマットを使う
------------------

今日では、Web アプリケーションは単なる HTML ページ以外の形式で応答することも要求されるようになっています。
RSS フィードのや Web サービス向けの XML 形式、Ajax リクエスト向けの JSON 形式など、さまざまなフォーマットが存在します。
Symfony2 におけるフォーマットのサポートは、とても単純です。
ルートを編集して、\ ``_format`` 変数のデフォルト値として ``xml`` を指定してみましょう。

::

    // src/Acme/DemoBundle/Controller/DemoController.php
    /**
     * @extra:Route("/hello/{name}", defaults={"_format"="xml"}, name="_demo_hello")
     * @extra:Template()
     */
    public function helloAction($name)
    {
        return array('name' => $name);
    }

``_format`` の値で定義されているリクエストのフォーマットを使って、Symfony2 が自動的に適切なテンプレートを決定します。
ここでは ``hello.xml.twig`` になります。

.. code-block:: xml+php

    <!-- src/Acme/DemoBundle/Resources/views/Demo/hello.xml.twig -->
    <hello>
        <name>{{ name }}</name>
    </hello>

これだけです。
標準的フォーマットについては、レスポンスに対してもっとも適切な ``Content-Type`` ヘッダーが Symfony2 によって自動的に選択されます。
単一のアクションで複数の異なるフォーマットをサポートしたい場合は、ルートパターンに ``{_format}`` プレースホルダを使います。

::

    // src/Acme/DemoBundle/Controller/DemoController.php
    /**
     * @extra:Route("/hello/{name}.{_format}", defaults={"_format"="html"}, requirements={"_format"="html|xml|json"}, name="_demo_hello")
     * @extra:Template()
     */
    public function helloAction($name)
    {
        return array('name' => $name);
    }

このコントローラは、\ ``/demo/hello/Fabien.xml`` や ``/demo/hello/Fabien.json`` といった URL で呼び出されるようになります。

``requirements`` エントリには、プレースホルダの値をチェックするための正規表現を定義します。
この例では、もし ``/demo/hello/Fabien.js`` というリソースへのリクエストがあった場合、\ ``_format`` の正規表現にマッチしないため、HTTP の 404 エラーが返されます。

リダイレクトとフォワード
------------------------

ユーザーを別のページへリダイレクトさせたい場合は、\ ``redirect()`` メソッドを使います。

::

    return $this->redirect($this->generateUrl('_demo_hello', array('name' => 'Lucas')));

``generateUrl()`` メソッドの機能は、テンプレートで使った ``path()`` 関数と同じです。
ルート名とパラメータの配列を引数にとり、ルート名に関連付けられた使いやすい URL を生成して返します。

アクションから別のアクションへフォワードするのも同じように簡単で、\ ``forward()`` メソッドを使います。
内部的には、フォワード先のアクションへの "サブリクエスト" が作られ、そのサブリクエストを実行した結果の ``Response`` オブエジェクトが返されます。

::

    $response = $this->forward('AcmeDemoBundle:Hello:fancy', array('name' => $name, 'color' => 'green'));

    // $response に対して何らかの処理を行うか、そのまま return する

リクエストから情報を取得する
----------------------------

コントローラからアクセスできる値は、ルーティングのプレースホルダで設定した値以外に、\ ``Request`` オブジェクトがあります。

::

    $request = $this->get('request');

    $request->isXmlHttpRequest(); // Ajax のリクエストかどうか？

    $request->getPreferredLanguage(array('en', 'fr'));

    $request->query->get('page'); // $_GET のパラメータを取得

    $request->request->get('page'); // $_POST のパラメータを取得

テンプレートでは、\ ``app.request`` 変数を使って ``Request`` オブジェクトにアクセスできます。

.. code-block:: html+jinja

    {{ app.request.query.get('page') }}

    {{ app.request.parameter('page') }}

セッションにデータを格納する
----------------------------

HTTP プロトコルはステートレスですが、ブラウザを使っている実際の人、またはボットや Web 使いやすいセッションオブジェクトが Symfony2 には組み込まれています。
PHP ネイティブのセッション機能を使って実装されており、2 つのリクエストに渡って属性を保存できます。

セッションへの情報の保存とセッションからの情報の取得は、任意のコントローラから簡単に行なえます。

::

    $session = $this->get('request')->getSession();

    // 後続のユーザーからのリクエストで再利用するために属性を保存
    $session->set('foo', 'bar');

    // 別のコントローラにおける別のリクエストにて
    $foo = $session->get('foo');

    // ユーザーのロケールを設定
    $session->setLocale('fr');

直後のリクエストでのみ有効な小さなメッセージをセッションに保存することもできます。

::

    // 直後のリクエストでのみ利用可能なメッセージを保存（コントローラにて）
    $session->setFlash('notice', 'Congratulations, your action succeeded!');

    // 次のリクエストでメッセージを表示（テンプレートにて）
    {{ app.session.flash('notice') }}

この機能は、ユーザーを別のページへリダイレクトさせる前に処理の完了メッセージを設定し、リダイレクト先のページでメッセージを表示する必要がある場合に便利です。

リソースのセキュリティーを設定する
----------------------------------

Symfony Standard Edition には、よく使われる要件にあう単純なセキュリティーコンフィギュレーションが含まれています。

.. code-block:: yaml

    # app/config/security.yml
    security:
        encoders:
            Symfony\Component\Security\Core\User\User: plaintext

        role_hierarchy:
            ROLE_ADMIN:       ROLE_USER
            ROLE_SUPER_ADMIN: [ROLE_USER, ROLE_ADMIN, ROLE_ALLOWED_TO_SWITCH]

        providers:
            in_memory:
                users:
                    user:  { password: userpass, roles: [ 'ROLE_USER' ] }
                    admin: { password: adminpass, roles: [ 'ROLE_ADMIN' ] }

        firewalls:
            login:
                pattern:  /demo/secured/login
                security: false

            secured_area:
                pattern:    /demo/secured/.*
                form_login:
                    check_path: /demo/secured/login_check
                    login_path: /demo/secured/login
                logout:
                    path:   /demo/secured/logout
                    target: /demo/

このコンフィギュレーションでは、\ ``/demo/secured/`` で始まる任意の URL にアクセスしたユーザーにログインを要求するよう設定し、\ ``user`` と ``admin`` という 2 種類のユーザーを定義しています。
さらに、\ ``admin`` ユーザーには ``ROLE_USER`` ロールを含む ``ROLE_ADMIN`` ロールが付与されています（\ ``role_hierarchy`` 設定を参照してください）。

.. tip::

    可読性のために、この単純なコンフィギュレーションではパスワードが平文で記述されていますが、\ ``encoders`` セクションのコンフィギュレーションにより任意のハッシュアルゴリズムを設定できます。

``http://localhost/Symfony/web/app_dev.php/demo/secured/hello`` という URL へアクセスした場合、このリソースは\ ``ファイアウォール``\ で保護されているため、ユーザーは自動的にログインフォームへリダイレクトされます。

コントローラで ``@extra:Secure`` アノテーションを使って、アクションで任意のロールを要求するように設定することもできます。

::

    /**
     * @extra:Route("/hello/admin/{name}", name="_demo_secured_hello_admin")
     * @extra:Secure(roles="ROLE_ADMIN")
     * @extra:Template()
     */
    public function helloAdminAction($name)
    {
        return array('name' => $name);
    }

``user`` （このユーザーには ``ROLE_ADMIN`` ロールが付与されていない）でログインし、セキュリティーで保護された Hello ページから "Hello resource secured" リンクをクリックしてみてください。
Symfony2 により HTTP 403 ステータスコードが返されます。
これは、ユーザーが該当リソースへのアクセスを\ "拒否"\ されたことを示します。

.. note::

    Symfony2 セキュリティーレイヤーはとても柔軟で、たとえば Doctrine ORM 向けなどのさまざまなユーザープロバイダや、HTTP 基本認証、HTTP ダイジェスト認証、X509 証明書での認証といった認証プロバイダなどが組み込まれています。
    セキュリティーレイヤーの使い方と設定方法の詳細については、ガイドブックの ":doc:`/book/security/overview`" の章を参照してください。

リソースをキャッシュする
------------------------

構築したサイトのトラフィックが日に日に増えてくると、同一のリソースを何度も生成することを避けたいと考えるでしょう。
Symfony2 では HTTP キャッシュヘッダーを使ってリソースのキャッシュを管理できます。単純なキャッシュ戦略では、便利な ``@extra:Cache()`` アノテーションを使います。

::

    /**
     * @extra:Route("/hello/{name}", name="_demo_hello")
     * @extra:Template()
     * @extra:Cache(maxage="86400")
     */
    public function helloAction($name)
    {
        return array('name' => $name);
    }

この例では、リソースは 1 日キャッシュされます。
コンテンツの要件に合わせて単に期限を指定するのではなくバリデーションを使ったり、期限とバリデーションを組み合わせて使うこともできます。

リソースのキャッシュは、Symfony2 に組み込まれたリバースプロキシで制御されます。
また、一般的な HTTP キャッシュヘッダーを使ってキャッシュの制御を行うようになっているため、組み込みのリバースプロキシの代わりに Varnish や Squid に置き換えることもでき、アプリケーションを容易にスケールさせられます。

.. note::

    ページ全体をキャッシュできない場合はどうするのでしょうか？
    Symfony2 には Edge Side Include (ESI) を使ったソリューションもあり、これもネイティブで組み込まれています。
    キャッシュや ESI の詳細については、ガイドブックの ":doc:`/book/http_cache`" の章を参照してください。

まとめ
------

この章はこれで終わりです。
10 分もかからなかったのではないでしょうか。
最初の章でバンドルという概念を簡単に解説したのを覚えていますか？
私たちが今学んでいる機能は、コアのフレームワークバンドルの機能の一部なのです。
バンドルの仕組みがあるおかげで、Symfony2 のすべての機能は拡張可能かつ置き換え可能です。
これが、このチュートリアルの次の章のトピックです。
