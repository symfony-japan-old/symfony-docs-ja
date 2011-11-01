.. index::
   single: Templating; Global variables

変数を全てのテンプレートへ注入する(グローバル値)
==============================================================

使用している全てのテンプレートからアクセスできる変数が必要なときもあります。これは、 ``app/config/config.yml`` ファイルの内部で指定することで可能です。

.. code-block:: yaml

    # app/config/config.yml
    twig:
        # ...
        globals:
            ga_tracking: UA-xxxxx-x

これで、 ``ga_tracking`` の変数が全ての Twig のテンプレートで使用可能になりました。

.. code-block:: html+jinja

    <p>Our google tracking code is: {{ ga_tracking }} </p>

とても簡単でした。ビルトインシステム :ref:`book-service-container-parameters` のアドバンテージを受けることもできます。そうすれば、値を孤立化できますし、また、再利用もできるようになります。

.. code-block:: ini

    ; app/config/parameters.yml
    [parameters]
        ga_tracking: UA-xxxxx-x

.. code-block:: yaml

    # app/config/config.yml
    twig:
        globals:
            ga_tracking: %ga_tracking%

前と全く同じ変数が使用可能になりました。

さらに複雑なグローバル値
-----------------------------

使用したいグローバル値がより複雑だった際には(例えばオブジェクト)、上記の方法を使用することはできません。代わりに、 :ref:`Twig Extension<reference-dic-tags-twig-extension>` を作成し、グローバル値を ``getGlobals`` メソッドのエントリの１つとして返す必要があります。

.. 2011/11/02 ganchiku 5d5cdf2d6994294fb7b867cb5a53f6fedbb9984d

