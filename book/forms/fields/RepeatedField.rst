RepeatedField
=============

``RepeatedField`` は、フィールドを2回出力できるよう拡張されたフィールドグループです。繰り返しフィールドでは、ユーザーが両方のフィールドに同じ値を入れた時だけバリデーションが実施されます。

.. code-block:: php

    use Symfony\Component\Form\RepeatedField;

    $form->add(new RepeatedField(new TextField('email')));

これは、Eメールアドレスやパスワードを問い合わせる時にとても便利です！
