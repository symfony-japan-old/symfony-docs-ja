Date
====

Validates that a value is a valid date string with format "YYYY-MM-DD".

値が "YYYY-MM-DD" フォーマットの日にち文字列であることのバリデートを実行します。

.. code-block:: yaml

    properties:
        birthday:
            - Date: ~

オプション
-------

.. * ``message``: The error message if the validation fails

* ``message``: バリデーションが失敗した時のエラーメッセージ。
