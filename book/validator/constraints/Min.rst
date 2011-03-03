Min
===

.. Validates that a value is not smaller than the given limit.

与えられた制限値よりも値が小さくないことのバリデートを実行します。

.. code-block:: yaml

    properties:
        age:
            - Min: 1

オプション
----------

.. * ``limit`` (**default**, required): The limit
   * ``message``: The error message if validation fails

* ``limit`` (**デフォルト**, 必須): 制限値。
* ``message``: バリデーションが失敗した時のエラーメッセージ。
