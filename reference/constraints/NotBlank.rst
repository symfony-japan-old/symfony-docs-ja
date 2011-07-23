.. 2011/07/23 yanchi 36a165e88363fd6e5b5eb0ae712303dd362545be

NotBlank
========

値が空ではないことを検証します。(`empty <http://php.net/empty>`_construct によって決定します)。

.. code-block:: yaml

    properties:
        firstName:
            - NotBlank: ~

Options
-------

* ``message``: 検証に失敗した場合のエラーメッセージ
