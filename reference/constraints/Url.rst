.. 2011/07/23 yanchi 36a165e88363fd6e5b5eb0ae712303dd362545be

Url
===

値が有効なURL文字列であることを検証します。

.. code-block:: yaml

    properties:
        website:
            - Url: ~

オプション
----------

* ``protocols``: 許可されているプロトコルのリスト。 デフォルト: "http", "https", "ftp", "ftps".
* ``message``: 検証に失敗した場合のエラーメッセージ
