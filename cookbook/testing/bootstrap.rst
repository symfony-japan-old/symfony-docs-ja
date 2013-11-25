.. index::
   single: Tests; Bootstrap

テスト実行前のブートストラップ処理のカスタマイズ方法
====================================================

テストを実行する前に、何らかの前処理（ブートストラップ処理）が必要な場合があります。
例えば、新しい翻訳 (Translation) を追加した後でファンクショナルテストを実行する場合、キャッシュのクリアが必要です。
このクックブックではその方法について記述します。

まず、以下のようなカスタムブートストラップファイルを作成します。
（訳注：これは、環境変数 ``$ENV['BOOTSTRAP_CLEAR_CACHE_ENV']`` に設定された環境のキャッシュをクリアして、オリジナルのブートストラップファイルをロードするだけの、簡単なカスタムブートストラップファイルです。）

.. code-block:: php

    // app/tests.bootstrap.php
    if (isset($_ENV['BOOTSTRAP_CLEAR_CACHE_ENV'])) {
        // コンソールコマンド cache:clear を実行
        passthru(sprintf(
            'php "%s/console" cache:clear --env=%s --no-warmup',
            __DIR__,
            $_ENV['BOOTSTRAP_CLEAR_CACHE_ENV']
        ));
    }

    // オリジナルのブートストラップファイルをロード
    require __DIR__.'/bootstrap.php.cache';

次に、PHPUnit の設定ファイル ``app/phpunit.xml.dist`` を修正して、\ ``bootstrap`` 属性の値を、今作ったファイル名 "``tests.bootstrap.php``" で置換します。

.. code-block:: xml

    <!-- app/phpunit.xml.dist -->

    <!-- ... -->
    <phpunit
        ...
        bootstrap = "tests.bootstrap.php"
    >

同じく ``app/phpunit.xml.dist`` に環境変数 ``BOOTSTRAP_CLEAR_CACHE_ENV`` の値を設定して、どの環境のキャッシュをクリアするかを定義します。

.. code-block:: xml

    <!-- app/phpunit.xml.dist -->
    <php>
        <env name="BOOTSTRAP_CLEAR_CACHE_ENV" value="test"/>
    </php>

これで、テストの実行前に ``test`` 環境のキャッシュをクリアするブートストラップ処理が走るようになります。

.. 2013/11/25 monmonmon 6ea490d50011e9c114e815b9e68b5ff29c1d71ae
