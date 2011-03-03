DateTime
========

.. Validates that a value is a valid datetime string with format "YYYY-MM-DD HH:MM:SS".

値が "YYYY-MM-DD HH:MM:SS" フォーマットでの有効な日時文字列であることのバリデーションを実行します。

.. code-block:: yaml

    properties:
        createdAt:
            - DateTime: ~

オプション
-------

.. * ``message``: The error message if the validation fails

* ``message``: バリデーションが失敗した時のエラーメッセージ。
