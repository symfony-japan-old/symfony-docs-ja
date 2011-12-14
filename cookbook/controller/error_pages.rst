エラーページのカスタマイズ方法
==============================

Symfony2 内で例外が投げられると、その例外は ``Kernel`` クラスでキャッチされ、最終的に例外を処理する特別なコントローラである ``TwigBundle:Exception:show`` にフォワードされます。このコントローラは、 ``TwigBundle`` のコアにあり、受け取った例外から、表示するエラーテンプレートやセットすべきステータスコードを決定します。

エラーページは、２つの方法でカスタマイズをすることができますが、制御したいレベルによってどちらかを選んでください。

1. この記事で説明する内容で、異なるエラーページのエラーテンプレートをカスタマイズします。

2. デフォルトのエラーコントローラである ``TwigBundle::Exception:show`` を自分で作ったコントローラに置き換えて、例外を好きなように処理します。(\ :ref:`exception_controller in the Twig reference<config-twig-exception-controller>` を参照してください)

.. tip::

    ２つ目の方法である例外処理のカスタマイズの方が、１つ目の方法よりも、より強力です。内部のイベントである ``kernel.exception`` が投げられ、例外処理をすべて制御できるからです。詳細は、\ :ref:`kernel-kernel.exception` を参照してください。

全てのエラーテンプレートは、\ ``TwigBundle`` 内にあります。これらのテンプレートをオーバーライドするには、制御したいバンドル内で、テンプレートのオーバーライドするための標準メソッドを呼べば良いのです。詳細は、\ :ref:`overriding-bundle-templates` を参照してください。

例として、エンドユーザに表示するデフォルトのエラーテンプレートをオーバーライドしてみます。新しいテンプレートを ``app/Resources/TwigBundle/views/Exception/error.html.twig`` に作成してください。

.. code-block:: html+jinja

    <!DOCTYPE html>
    <html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <title>An Error Occurred: {{ status_text }}</title>
    </head>
    <body>
        <h1>Oops! An Error Occurred</h1>
        <h2>The server returned a "{{ status_code }} {{ status_text }}".</h2>
    </body>
    </html>

.. tip::

    Twig に慣れていなくても大丈夫です。Twig は ``Symfony2`` に統合されている簡単で強力なテンプレートエンジンです。Twig の詳細は :doc:`/book/templating` を参照してください。

標準的な HTML のエラーページに加え、Symfony はたくさんの一般的なレスポンスフォーマットのデフォルトエラーページを提供します。例を上げると、 JSON(``error.json.twig``)、XML(``error.xml.twig``)、JavaScript(``error.js.twig``) などです。これらのテンプレートをオーバーライドするには、同じ名前のテンプレートファイルを ``app/Resources/TwigBundle/views/Exception`` ディレクトリに作成すればいいのです。また、この方法は、バンドル内にあるエラーテンプレートのみの方法ではなく、あらゆるテンプレートをオーバーライドする標準的な方法になります。

.. _cookbook-error-pages-by-status-code:

404 ページや他のエラーページをカスタマイズする
----------------------------------------------

HTTP のステータスコードに準じて、特定のエラーテンプレートをカスタマイズすることもできます。例えば、\ ``app/Resources/TwigBundle/views/Exception/error404.html.twig`` テンプレートを作成して Page not Found の 404 エラーのための特別なページを表示させることができます。

Symfony は、次のアルゴリズムを使用し、どのテンプレートを使うか決定します。

* まず、フォーマットとステータスコードから一致するテンプレートを探します(``error404.json.twig`` のように)。

* もし一致するテンプレートが無ければ、フォーマットに一致するテンプレートを探します(``error.json.twig`` のように)。

* それでも一致するテンプレートが無ければ、HTML テンプレートを使用します(``error.html.twig`` のように)

.. tip::

    デフォルトエラーテンプレートの一覧は、\ ``TwigBundle`` 内の ``Resources/views/Exception`` ディレクトリにあります。標準的な Symfony2 のインストールでは、\ ``TwigBundle`` は ``vendor/symfony/src/Symfony/Bundle/TwigBundle`` にあります。エラーページをカスタマイズするための一番簡単な方法は、\ ``TwigBundle`` からカスタマイズしたいエラーページを ``app/Resources/TwigBundle/views/Exception`` にコピーして、それを元にカスタマイズすることです。

.. note::

    開発者のためのデバッグフレンドリーな例外ページも、テンプレートを作成する方法と同じようにカスタマイズすることができます。例えば、標準的な HTML 例外ページは ``exception.html.twig`` で、JSON 例外ページは ``exception.json.twig`` になります。

.. 2011/10/24 ganchiku 2067a87287f11466b660616642bd4a5e58568a43

