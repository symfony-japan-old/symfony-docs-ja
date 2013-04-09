.. index::
   single: Templating
   single: Components; Templating

.. note::

    * 翻訳更新日：2013/04/09

Templatingコンポーネント
========================

    Templatingコンポーネントは、どんな種類のテンプレートシステムを作るのにも使うことができる全てのツールを提供します。

    テンプレートファイルを読み込む仕組みはもちろん、使いたい場合はテンプレートファイルの変更を監視する仕組みを使うこともできます。PHPを使ったテンプレートエンジンもエスケープとテンプレートをブロックとレイアウトに分離する仕組みも含んでいます。

インストール
------------

コンポーネントをインストールする方法は何通りもあります。

* 公式Gitレポジトリ (https://github.com/symfony/Templating)
* :doc:`Composer</components/using_components>` (`Packagist`_ の ``symfony/templating``)

使い方
-------

:class:`Symfony\\Component\\Templating\\PhpEngine` クラスはコンポーネントのエントリポイントです。このクラスは、テンプレート名をテンプレートリファレンスに変換するためのテンプレートネームパーサー( :class:`Symfony\\Component\\Templating\\TemplateNameParserInterface` )と、テンプレートリファレンスに対応するテンプレートファイルを見つけるためのテンプレートローダー( :class:`Symfony\\Component\\Templating\\Loader\\LoaderInterface` )を要求します。
::

    use Symfony\Component\Templating\PhpEngine;
    use Symfony\Component\Templating\TemplateNameParser;
    use Symfony\Component\Templating\Loader\FilesystemLoader;

    $loader = new FilesystemLoader(__DIR__ . '/views/%name%');

    $view = new PhpEngine(new TemplateNameParser(), $loader);

    echo $view->render('hello.php', array('firstname' => 'Fabien'));

:method:`Symfony\\Component\\Templating\\PhpEngine::render` メソッドは `views/hello.php` ファイルを実行し、出力内容を返します。

.. code-block:: html+php

    <!-- views/hello.php -->
    Hello, <?php echo $firstname ?>!

スロットを利用したテンプレートの継承
-------------------------------------

テンプレートの継承により、多くのテンプレートの間でレイアウトを共通化することができます。

.. code-block:: html+php

    <!-- views/layout.php -->
    <html>
        <head>
            <title><?php $view['slots']->output('title', 'Default title') ?></title>
        </head>
        <body>
            <?php $view['slots']->output('_content') ?>
        </body>
    </html>

:method:`Symfony\\Component\\Templating\\PhpEngine::extend` メソッドは、個別のテンプレートの中で親テンプレートを指定するときに使います。

.. code-block:: html+php

    <!-- views/page.php -->
    <?php $view->extend('layout.php') ?>

    <?php $view['slots']->set('title', $page->title) ?>

    <h1>
        <?php echo $page->title ?>
    </h1>
    <p>
        <?php echo $page->body ?>
    </p>

テンプレート継承を使うには、 :class:`Symfony\\Component\\Templating\\Helper\\SlotsHelper` ヘルパーを有効化する必要があります。
::

    use Symfony\Component\Templating\Helper\SlotsHelper;

    $view->set(new SlotsHelper());

    // ページのインスタンスを取得
    $page = ...;

    echo $view->render('page.php', array('page' => $page));

.. note::

    テンプレートは多重継承できます。つまり、レイアウトのテンプレートは他のレイアウトを継承できます。

出力エスケープ
---------------

この章はまだ執筆途中です。

アセットヘルパー
----------------

この章はまだ執筆途中です。

.. _Packagist: https://packagist.org/packages/symfony/templating

.. 2013/04/09 77web dd39b93d9c487ede4f8988e858af4926d720a674