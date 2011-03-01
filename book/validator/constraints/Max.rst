Max
===

.. Validates that a value is not greater than the given limit.

与えられた制限値よりも値が大きくないことのバリデーションを実行します。

.. code-block:: yaml

    properties:
        age:
            - Max: 99

オプション
-------

.. * ``limit`` (**default**, required): The limit
   * ``message``: The error message if validation fails

* ``limit`` (**デフォルト**, 必須): 制限値。
* ``message``: バリデーションが失敗した時のエラーメッセージ。
