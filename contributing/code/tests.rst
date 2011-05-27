.. 2011/05/18 doublemarket fb4f4ce4

Symfony2 のテストを実行する
===========================

:doc:`パッチ <patches>`\ を投稿する前に、必ず Symfony2 のテストスイートを実行し、予期せぬ不具合が発生していないことを確認してください。

PHPUnit
-------

Symfony2 のテストスイートを実行するには、PHPUnit 3.5.0 以降が\ `インストール`_\ されている必要があります。

.. code-block:: bash

    $ pear channel-discover pear.phpunit.de
    $ pear channel-discover components.ez.no
    $ pear channel-discover pear.symfony-project.com
    $ pear install phpunit/PHPUnit

依存関係（任意）
----------------

外部に依存関係のあるテストも含めたテストスイート全体を実行するには、Symfony2 でそれらをオートロードできるようにしておく必要があります。デフォルトでは、外部依存ライブラリは、メインのルートディレクトリ以下の `vendor/` ディレクトリから読み込まれます（\ `autoload.php.dist`\ ）。

テストスイートの実行には、次のサードパーティライブラリが必要になります。

* Doctrine
* Doctrine Migrations
* Swiftmailer
* Twig

これらのすべてをインストールするには、\ `install_vendors.sh` スクリプトを実行します。

.. code-block:: bash

    $ sh vendors.sh

.. note::

    このスクリプトの実行が完了するまで、しばらく時間がかかります。

一度インストールが完了している環境であれば、\ `vendors.sh` スクリプトを再度実行していつでも vendor を更新できます。

実行
----

最初に、vendor を最新のものに更新してください（上で説明した手順を参照してください）。

次に、Symfony2 のルートディレクトリへ移動し、次のコマンドでテストスイートを実行します。

.. code-block:: bash

    $ phpunit

出力に `OK` と表示されているはずです。そうでない場合は問題の箇所を特定し、問題の原因が自分の修正によるものなのかどうかを確認する必要があります。

.. tip::

    変更を適用する前にテストスイートを実行しておき、自分の環境でテストスイートが正しく実行されることを確認しておくことをおすすめします。

コードカバレッジ
----------------

新しい機能を追加したら、\ `coverage-html` オプションを使ってコードカバレッジも確認してください。

.. code-block:: bash

    $ phpunit --coverage-html=cov/

`cov/index.html` に生成されたページをブラウザで開いて、コードカバレッジを確認してください。

.. tip::

    コードカバレッジの機能を利用するには、XDebug とすべての依存ライブラリがインストールされている必要があります。

.. _インストール: http://www.phpunit.de/manual/current/ja/installation.html
