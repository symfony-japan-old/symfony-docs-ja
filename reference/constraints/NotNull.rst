.. 2011/07/23 yanchi 36a165e88363fd6e5b5eb0ae712303dd362545be

NotNull
=======

値が``null``ではないことを検証します。

.. code-block:: yaml

    properties:
        firstName:
            - NotNull: ~

オプション
----------

* ``message``: 検証に失敗した場合のエラーメッセージ
