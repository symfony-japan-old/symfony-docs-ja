.. index::
   single: Routing; Configure redirect to another route without a custom controller

カスタムコントローラを使用せずに別のルートへリダイレクトする方法
========================================================================

このガイドでは、カスタムコントローラを使用せずに1つのルートから別のルートにリダイレクトを設定する方法について説明します。

``/`` パスに対する有効なデフォルトコントローラが存在せずに ``/app`` へリクエストをリダイレクトしたい場合を仮定します。

コンフィグレーションは次のようになります。

.. code-block:: yaml

    AppBundle:
        resource: "@App/Controller/"
        type:     annotation
        prefix:   /app

    root:
        pattern: /
        defaults:
            _controller: FrameworkBundle:Redirect:urlRedirect
            path: /app
            permanent: true

``AppBundle`` は ``/app`` 下へのリクエストを処理するよう登録されています。

``/`` パスへのルートを設定すると、`Symfony\\Bundle\\FrameworkBundle\\Controller\\RedirectController` クラスが処理を行います。このコントローラはリクエストをリダイレクトするための2つのメソッドが組み込まれています。

* ``redirect`` は別のルートへリダイレクトします。リダイレクトしたいルートの名前と ``route`` パラメータを指定する必要があります。

* ``urlRedirect`` は別のパスへリダイレクトします。リダイレクトしたいリソースのパスを含む ``path`` パラメータを指定する必要があります。

``permanent`` スイッチは両メソッドに301 HTTPステータスコードを指示します。

.. 2012/09/16 taka512 ed4ea54e486600e07f17215b0b2999730f0666e1
