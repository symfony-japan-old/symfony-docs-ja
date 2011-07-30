.. 2011/07/30 hidenorigoto 64eee44fe362b5dffe1f0f679b1822b72dd03a6b

.. index::
   single: Requirements

Symfony2 の実行に必要な要件
===========================

Symfony2 を実行するには、いくつかの実行要件を満たす必要があります。
Symfony ディストリビューションに同梱されている ``web/config.php`` に Web ブラウザからアクセスすると、お使いのシステムがすべての要件を満たしているかどうか、簡単にテストできます。
ただし、PHP の CLI 環境は、Web サーバー環境とは異なる ``php.ini`` ファイルを使っている場合があるため、コマンドラインからも次のプログラムを実行して要件を確認しておくことをお勧めします。

.. code-block:: bash

    php app/check.php

必須および任意の要件の一覧は、次のとおりです。

必須
----

* PHP 5.3.2 か、それ以降のバージョンがインストールされていること
* PHP.ini で date.timezone の設定があること

任意
----

* PHP-XML モジュールがインストールされていること
* libxml の 2.6.21 か、それ以降のバージョンがインストールされていること
* PHP tokenizer が有効化されていること
* mbstring 関数群が有効化されていること
* iconv が有効化されていること
* POSIX が有効化されていること
* Intl がインストールされていること
* APC または他の OPCODE キャッシュシステムがインストールされていること
* PHP.ini の推奨設定は次のとおりです

    * short_open_tags: off
    * magic_quotes_gpc: off
    * register_globals: off
    * session.autostart: off

Doctrine
--------

Doctrine を使う場合は、PDO がインストールされている必要があります。
また、データベースサーバー側にも PDO ドライバがインストールされている必要があります。
