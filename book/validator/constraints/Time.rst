Time
====

.. Validates that a value is a valid time string with format "HH:MM:SS".

値が "HH:MM:SS" のフォーマットを持つ有効な時間の文字列であることのバリデーションを実行します。

.. code-block:: yaml

    properties:
        createdAt:
            - DateTime: ~

オプション
----------

.. * ``message``: The error message if the validation fails

* ``message``: バリデーションが失敗した時のエラーメッセージ。
