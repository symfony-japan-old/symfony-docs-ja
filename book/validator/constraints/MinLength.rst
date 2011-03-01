MinLength
=========

.. Validates that the string length of a value is not smaller than the given limit.

与えられた制限値よりも文字列の長さが短いことのバリデーションを実行します。

.. code-block:: yaml

    properties:
        firstName:
            - MinLength: 3

オプション
-------

.. * ``limit`` (**default**, required): The limit
   * ``message``: The error message if validation fails

* ``limit`` (**デフォルト**, 必須): 制限値。
* ``message``: バリデーションが失敗した時のエラーメッセージ。
