NotNull
=======

.. Validates that a value is not ``null``.

値が ``null`` でないことのバリデーションを実行します。

.. code-block:: yaml

    properties:
        firstName:
            - NotNull: ~

オプション
-------

.. * ``message``: The error message if validation fails

* ``message``: バリデーションが失敗した時のエラーメッセージ。
