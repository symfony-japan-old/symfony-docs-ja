.. 2011/07/30 hidenorigoto 64eee44fe362b5dffe1f0f679b1822b72dd03a6b
.. 2014/05/02 77web        d49d12eaf265a5d6d32ac660c62f385d57261475

.. index::
   single: Requirements

.. note::

    * 対象バージョン: 2.3以上
    * 翻訳更新日: 2014/05/02


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

* PHP 5.3.3 か、それ以降のバージョンがインストールされていること
* json が有効になっていること
* ctype が有効になっていること
* ``php.ini`` に ``date.timezone`` の設定があること

.. caution::
    
    PHP 5.3.8 以下と 5.3.16 の環境では、 Symfony2 の実行に制限があります。
    詳しくは `READMEの要件セクション`_ をお読みください。

任意
----

* PHP-XML モジュールがインストールされていること
* libxml の 2.6.21 か、それ以降のバージョンがインストールされていること
* PHP tokenizer が有効化されていること
* mbstring 関数群が有効化されていること
* iconv が有効化されていること
* POSIX 拡張が有効化されていること（ \*nix 系OSの場合）
* ICU 4以上にリンクした Intl がインストールされていること
* APC 3.0.17以上または他の OPCODE キャッシュシステムがインストールされていること
* ``php.ini`` の推奨設定は次のとおりです

    * ``short_open_tag = off``
    * ``magic_quotes_gpc = off``
    * ``register_globals = off``
    * ``session.auto_start = off``

Doctrine
--------

Doctrine を使う場合は、PDO がインストールされている必要があります。
また、使用するデータベースサーバーに対応するPDO ドライバもインストールされている必要があります。

.. _`READMEの要件セクション`: https://github.com/symfony/symfony#requirements

