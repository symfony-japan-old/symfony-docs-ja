PHPをテンプレートエンジンとして使う
===================================

Symfony2では、Twigを標準のテンプレートエンジンとして利用しています。ただし、通常のPHPコードをテンプレートエンジンとして利用することもできます。この場合の機能的な違いはなく、Symfony2では同等に扱うことができます。テンプレートエンジンとしてPHPを使う場合にも、より強力に記述する仕組みをSymfony2が提供しています。

PHPのテンプレートを出力する
---------------------------

TwigでなくPHPのテンプレートを利用するには、テンプレートの名称 ``.twig`` ではなく ``.php`` に変更します。たとえば下記のコントローラは ``index.html.php`` テンプレートを出力します::

    // src/Sensio/HelloBundle/Controller/HelloController.php

    public function indexAction($name)
    {
        return $this->render('HelloBundle:Hello:index.html.php', array('name' => $name));
    }

.. index::

  single: Templating; Layout
  single: Layout

テンプレートのデコレーション
----------------------------

ヘッダーやフッターなど、プロジェクト共通で画面の一部を共有することが良くあります。Symfony2では、この仕組みを一般的な方法とは異なった方法で解決したいと思います。それはテンプレートのデコレーションという仕組みで、一つのテンプレートは別のテンプレートにデコレーションされる、という考え方です。

たとえば ``index.html.php`` テンプレートが、 ``layout.html.php`` にデコレーションされる例を掲載します。この場合、 ``extend()`` メソッドを呼び出して下記の通り記述します。

.. code-block:: html+php

    <!-- src/Sensio/HelloBundle/Resources/views/Hello/index.html.php -->
    <?php $view->extend('HelloBundle::layout.html.php') ?>

    Hello <?php echo $name ?>!

この ``HelloBundle::layout.html.php`` という表記は、他のテンプレートファイルを参照する場合と同じ表記となります。 ``::`` の部分は、コントローラ要素が空白となっているため、該当するファイルは ``views/`` ディレクトリの直下に格納されている必要があります。

``layout.html.php`` ファイルの内容は下記の通りとなります。

.. code-block:: html+php

    <!-- src/Sensio/HelloBundle/Resources/views/layout.html.php -->
    <?php $view->extend('::base.html.php') ?>

    <h1>Hello Application</h1>

    <?php $view['slots']->output('_content') ?>

このレイアウトは、別のファイル(``::base.html.php``)からデコレーションされています。

このように、Symfony2では複数階層にわたるデコレーションに対応しており、一つのレイアウトファイルが他のファイルからデコレーションされることも可能です。

また、このようにテンプレート名のバンドル部分に記述がない場合、該当するビューは ``app/views/`` ディレクトリに配置されていることを示します。このディレクトリには、プロジェクトに共通のレイアウトファイルを格納すると良いでしょう。下記に ``base.html.php`` ファイルの例を示します。

.. code-block:: html+php

    <!-- app/views/base.html.php -->
    <!DOCTYPE html>
    <html>
        <head>
            <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
            <title><?php $view['slots']->output('title', 'Hello Application') ?></title>
        </head>
        <body>
            <?php $view['slots']->output('_content') ?>
        </body>
    </html>

両方のレイアウトについて ``$view['slots']->output('_content')`` と記述ある部分は子テンプレートの内容に差し替えられます。今回の例では、それぞれ ``index.html.php`` と ``layout.html.php`` ファイルの内容となります。

コードの中には ``$view`` というオブジェクトが使われています。このように、Symfony2のテンプレートでは ``$view`` 変数が常に用意されており、テンプレートエンジンの動作を手助けする多くのメソッドを提供しています。

.. index::
   single: Templating; Slot
   single: Slot

スロットを使う
--------------

スロットはコードの一部をテンプレート同士で受け渡す仕組みです。スロットは、テンプレート内で定義し、そのテンプレートをデコレートしたレイアウトから呼び出すことができます。

たとえば ``index.html.php`` テンプレートで、下記の通り ``title`` スロットに値をセットします。

.. code-block:: html+php

    <!-- src/Sensio/HelloBundle/Resources/views/Hello/index.html.php -->
    <?php $view->extend('HelloBundle::layout.html.php') ?>

    <?php $view['slots']->set('title', 'Hello World Application') ?>

    Hello <?php echo $name ?>!

次にレイアウトファイルにて、セットされたスロットを出力する記述を行います。

.. code-block:: html+php

    <!-- app/views/layout.html.php -->
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <title><?php $view['slots']->output('title', 'Hello Application') ?></title>
    </head>

このように ``set()`` メソッドでスロットに値をセットし、 ``output()`` メソッドでスロットの内容を埋め込みます。このとき、スロットに値がセットされていない場合は ``output()`` メソッドの第2引数に、デフォルト値を定義することもできます。

他にも ``_content`` という特別なスロットが定義されており、描画される子テンプレートの内容が含まれています。

長い文字を含むスロットを作成したい場合は、下記のように ``start()`` メソッドと ``stop()`` メソッドを活用した構文も利用できます。

.. code-block:: html+php

    <?php $view['slots']->start('title') ?>
        Some large amount of HTML
    <?php $view['slots']->stop() ?>

.. index::
   single: Templating; Include

別のテンプレートを取り込む
--------------------------

テンプレートの内容を部分的に共有するには、共有する部分だけをまとめたテンプレートを定義し、別のテンプレートから取り込むと便利です。

ここでは ``hello.html.php`` テンプレートを作成します。

.. code-block:: html+php

    <!-- src/Sensio/HelloBundle/Resources/views/Hello/hello.html.php -->
    Hello <?php echo $name ?>!

次に ``index.html.php`` テンプレートを書き換え、 ``hello.html.php`` ファイルを取り込むように記述します。

.. code-block:: html+php

    <!-- src/Sensio/HelloBundle/Resources/views/Hello/index.html.php -->
    <?php $view->extend('HelloBundle::layout.html.php') ?>

    <?php echo $view->render('HelloBundle:Hello:hello.html.php', array('name' => $name)) ?>

``render()`` メソッドでは、コードの内容を評価し、別のテンプレートの結果を返します。この仕組みは、コントローラで使われている方法と同じものです。

.. index::
   single: Templating; Embedding Pages



別のコントローラを取り込む
--------------------------

Symfony2では、別のコントローラの実行結果をテンプレート内に取り込むことができます。これは、Ajax系の処理や、他のコントローラにある変数を取り込みたい場合に効果を発揮します。

たとえば ``fancy`` という名前のアクションを作成し、この実行結果を ``index.html.php`` テンプレートに取り込みたい場合には、下記のコードを記述します。

.. code-block:: html+php

    <!-- src/Sensio/HelloBundle/Resources/views/Hello/index.html.php -->
    <?php echo $view['actions']->render('HelloBundle:Hello:fancy', array('name' => $name, 'color' => 'green')) ?>

ここで ``HelloBundle:Hello:fancy`` の部分は、 ``Hello`` コントローラの ``fancy`` アクションを表しています。さて、その ``Hello`` コントローラは、下記のようなコードとなっています。::

    // src/Sensio/HelloBundle/Controller/HelloController.php

    class HelloController extends Controller
    {
        public function fancyAction($name, $color)
        {
            // create some object, based on the $color variable
            $object = ...;

            return $this->render('HelloBundle:Hello:fancy.html.php', array('name' => $name, 'object' => $object));
        }

        // ...
    }

さて、コントローラ内には ``$view['actions']`` 変数の定義が行われていません。実は、スロットの際に自動的に定義されていた ``$view['slots']`` 変数と同様、 ``$view['actions']`` 変数についても自動的に定義されます。この特別な変数については、次のセクションで詳しく解説します。

.. index::
   single: Templating; Helpers

テンプレート ヘルパを使う
-------------------------

Symfony2のテンプレート システムでは、ヘルパーという仕組みを通じて簡単に拡張することができます。ヘルパーは、テンプレートを処理する時に使う機能を提供するためのPHPオブジェクトです。たとえば、Symfony2では ``actions`` と ``slots`` の2つのヘルパーが内蔵されています。

ページ間のリンクを作成する
~~~~~~~~~~~~~~~~~~~~~~~~~~

Webアプリケーションでは、次ページへのリンクがないページは考えられません。テンプレート内にURLを直接記述する代わりに ``router`` ヘルパーを使うことで、アプリケーションのルーティング設定に応じて自動的にURLの生成が行われます。こうすることで、簡単にURL表記を変更することが可能になります。

.. code-block:: html+php

    <a href="<?php echo $view['router']->generate('hello', array('name' => 'Thomas')) ?>">
        Greet Thomas!
    </a>

``generate()`` メソッドでは、引数としてルート名とパラメータの配列を渡します。ルート名はルーティング設定で定義された名前で、パラメータにはルーティング設定で定義された値を指定するために利用します。たとえば、上記の ``hello`` ルートは下記のようなルーティング定義となっています。

.. code-block:: yaml

    # src/Sensio/HelloBundle/Resources/config/routing.yml
    hello: # The route name
        pattern:  /hello/{name}
        defaults: { _controller: HelloBundle:Hello:index }

画像、JavaScript、スタイルシートなどのアセットを活用する
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Symfony2では、アセットを簡単に扱うために ``assets`` タグを提供しています。

.. code-block:: html+php

    <link href="<?php echo $view['assets']->getUrl('css/blog.css') ?>" rel="stylesheet" type="text/css" />

    <img src="<?php echo $view['assets']->getUrl('images/logo.png') ?>" />

``assets`` ヘルパーの目的は、Webアプリケーションの汎用性をあげることにあります。このヘルパーを使うと、アプリケーションのルートディレクトリの場所を意識することなく変更できます。


出力エスケープ
--------------

PHPをテンプレートエンジンとする場合、ユーザーに表示する変数は、必ず変数のエスケープが必要です。::

    <?php echo $view->escape($var) ?>

このように ``escape()`` メソッドを使用すると、HTMLコンテキスト内に変数を埋め込むためのエスケープ処理が行われます。出力先のコンテキストは第2引数で変更できるため、たとえばJavaScript向けの出力を行う場合は、下記の通り、コンテキストを ``js`` に指定します::

    <?php echo $view->escape($var, 'js') ?>
