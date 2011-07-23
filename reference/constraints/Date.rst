.. 2011/07/23 yanchi 36a165e88363fd6e5b5eb0ae712303dd362545be

Date
====

値が「YYYY-MM-DD」形式による有効な日付文字列であることを検証します。

.. code-block:: yaml

    properties:
        birthday:
            - Date: ~

オプション
-------

* ``message``: 検証に失敗した場合のエラーメッセージ
