method
~~~~~~

.. versionadded:: 2.3
    ``method`` オプションは Symfony 2.3 から追加されました。

**データ型**: ``string`` **デフォルト**: ``POST``

このオプションでは、フォームのデータを送信するために使用されるHTTPメソッドを指定します。
その値は ``form`` 要素の ``method`` 属性として表示されます。
また、 ``handleRequest()`` メソッドの中で送信後、フォームの送信を処理するかどうかを決定するために使用されています。
可能な値は次のとおりです。
This option specifies the HTTP method used to submit the form's data. Its
value is rendered as the ``method`` attribute of the ``form`` element and is
used to decide whether to process the form submission in the
``handleRequest()`` method after submission. Possible values are:

* POST
* GET
* PUT
* DELETE
* PATCH

.. note:

    メソッドが PUT 、PATCH 、または DELETE の時、 Symfony は自動的にフォームに hidden フィールド ``_method`` をレンダリングします。
    これらは、"偽" のHTTPメソッドとして使用され、標準的なブラウザではサポートされていません。
    詳細については、以下を参照してください。
    :doc:`/cookbook/routing/method_parameters`

.. note:

    PATCH メソッドは部分データを送信することができます。
    言い換えれば、送信されたフォームデータの特定フィールドが欠落している場合、それらは無視され、デフォルト値が（もしあれば）使用されます。
    他のすべてのHTTPメソッドは、いくつかのフィールドが欠落している場合、それらのフィールドには ``null`` が設定されます。

.. 2014/06/20 yositani2002 72c30c2fa1af40af7a1f3e672005d360a898ca07
