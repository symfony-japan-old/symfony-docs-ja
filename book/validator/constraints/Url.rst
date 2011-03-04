Url
===

.. Validates that a value is a valid URL string.

値が有効なURL文字列であることのバリデーションを実行します。

.. code-block:: yaml

    properties:
        website:
            - Url: ~

オプション
----------

.. * ``protocols``: A list of allowed protocols. Default: "http", "https", "ftp"
      and "ftps".
   * ``message``: The error message if validation fails

* ``protocols``: 許可されるプロトコルの一覧。デフォルトでは、"http", "https", "ftp", "ftps"
* ``message``: バリデーションが失敗した時のエラーメッセージ。
